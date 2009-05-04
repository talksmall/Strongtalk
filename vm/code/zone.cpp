/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.39 $ */
/* Copyright (c) 2006, Sun Microsystems, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
	  disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of Sun Microsystems nor the names of its contributors may be used to endorse or promote products derived 
	  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT 
NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE


*/

# include "incls/_precompiled.incl"

#ifdef DELTA_COMPILER

# include "incls/_zone.cpp.incl"

# define LRU_RESOLUTION     16	/* resolution (in secs) of LRU timer */

# define LRU_RETIREMENT_AGE 10	/* min. age (# sweeps) for retirement */
# define LRU_MAX_VISITED    min(numberOfNMethods(), LRUMaxVisited)
				/* max. # nmethods visited per sweep */
# define LRU_MAX_RECLAIMED 250000 /* stop after having found this amount of */
				/* space occupied by replacement candidates */
# define LRU_CUTOFF	   32   /* max. length (bytes) of "small" methods */
# define LRU_SMALL_BOOST    3   /* small methods live this many times longer */
	 // (because access methods don't clear their unused bit)

# define COMPACT_OVERHEAD  0.05 /* desired max. overhead for zone compaction */


// Trade-offs for blocksizes/queue length below:
// - larger blocks mean larger internal fragmentation but less space 
//   overhead for the heap maps and increased effectiveness of the free lists
// - longer free lists cover a wider range of allocations but can slow down
//   allocation (when most lists are empty but still have to be scanned).
# define CODE_BLOCK	64	/* block size for nmethods */
# define PIC_BLOCK      32	/* block size for PICs */
# define FREE	20		/* # of free lists */

# define StubBlock  16		/* block size for PIC zone */
# define StubFree   20		/* # of free lists for PIC zone */

# define MaxExtFrag  0.05	/* max. tolerated ext. fragmentation */

# define FOR_ALL_NMETHODS(var)						      \
    for (nmethod *var = first_nm(); var; var = next_nm(var))


# define FOR_ALL_PICS(var)						      \
    for (PIC *var = first_pic(); var; var = next_pic(var))

inline int roundSize(int s, int blockSize) {
  return (s + blockSize - 1) / blockSize * blockSize;
}

LRUcount* LRUtable;	// for optimized methods
int* LRUflag;		// == LRUtable, just different type for convenience

static int LRUtime; 	    // virtual time; incremented after every full sweep

// We could directly run the sweeper from the interrupt handler, but
// this is tricky since the stack is in a strange state.  
void sweepTrigger() {
  if (UseLRUInterrupts) {
    Universe::code->_needsSweep = true;
    // %note: should be:
    // currentProcess->setupPreemption();
  };
}

static void idOverflowError(int delta) {
  Unused(delta);
  // fix this - maybe eliminate nmethod IDs altogether?
  fatal("zone: nmethod ID table overflowed");
}

zone::zone(int& size) {
  methodHeap  = new Heap(Universe::current_sizes.code_size,     CODE_BLOCK);
  picHeap     = new Heap(Universe::current_sizes.pic_heap_size, PIC_BLOCK);

  methodTable = new codeTable(codeTableSize);

  // LRUflag = idManager->data;
  LRUtable = (LRUcount*)LRUflag;
  clear();
  
  // start sweeper
  // cpuTimer->enroll(sweepTrigger, 1.0 / LRU_RESOLUTION);
}

void zone::clear() {
  methodHeap->clear();
  picHeap->clear();
  methodTable->clear();

  LRUhand = NULL;
  LRUtime = 0;
  jump_table()->init();
  _needsCompaction = _needsSweep = false;
  compactTime = 0; compactDuration = 1; minFreeFrac = 0.05;
}

int zone::nextNMethodID() { return jump_table()->peekID(); }

nmethod* zone::allocate(int size) {
  // CSect cs(profilerSemaphore); // for profiler
  // must get method ID here! (because reclaim might change firstFree)
  // (compiler may have used peekID to get ID of new nmethod for LRU stuff)
  if (needsSweep()) doSweep();

  nmethod* n = (nmethod*) methodHeap->allocate(size);
  
  if (n == NULL) {
    // allocation failed so flush zombies and retry
    flushZombies();
    n = (nmethod*) methodHeap->allocate(size);
    if (n == NULL) {
      print();
      fatal("cannot allocate enough space for nmethod");
    }
  }

  verify_if_often();
  return n;
}

int zone::used() {
  return methodHeap->usedBytes();
}

void zone::verify_if_often() {
  if (VerifyZoneOften) {
    methodHeap->verify();
  }
}

void zone::flush() {
  ResourceMark rm;
  TraceTime   t("Flushing method cache...", PrintCodeReclamation);
  EventMarker em("flushing method cache");

  // Deoptimize all stack frames
  Processes::deoptimize_all();
 
  FOR_ALL_NMETHODS(p) { p->makeZombie(true); }
  flushZombies(false);

  verify_if_often();
}


int zone::findReplCandidates(int needed) {
  // find replacement candidates; stop if > needed bytes found or if
  // there seem to be no more candidates
 # ifdef NOT_IMPLEMENTED
  int reclaimed = 0, iter = 0;
  while (iter++ < LRU_RETIREMENT_AGE && reclaimed < needed) {	
    int vis, recl;
    int limit = numberOfNMethods();		// because usedIDs may change
    int newTime = sweeper(limit, needed, &vis, &recl);
    reclaimed += recl;
    if (recl < needed && newTime > LRUtime + 1) {
      // next sweep wouldn't reclaim anything
      assert(vis == limit, "should have visited them all");
      LRUtime = newTime - 1;
      if (PrintLRUSweep) lprintf("\n*forced new LRU time: %ld", LRUtime);
    }
  }
  return reclaimed;
#endif
  return 0;
}

// flush next replacement candidate; return # bytes freed
int zone::flushNextMethod(int needed) {
  int freed = 0;
  Unimplemented();
  return freed;
}

void moveInsts(char* from, char* to, int size) {
  Unused(size);
  nmethod* n = (nmethod*) from;
  nmethod* nTo = (nmethod*)to;
  
  char* n1 = n->insts();
  char* n2 = n->instsEnd();
  n->moveTo(to, (char*)n->locsEnd() - (char*)n);
  if (Universe::code->LRUhand == n) Universe::code->LRUhand = nTo;
}

class ConvertBlockClosure : public ObjectClosure {
 public:
  void do_object(memOop obj) {
    if (obj->is_block() && blockClosureOop(obj)->isCompiledBlock()) {
      blockClosureOop(obj)->deoptimize();
    }
  }
};

static nmethod* debug_nm = NULL;
extern nmethod* recompilee;
void zone::flushZombies(bool deoptimize) {
  // 1. cleanup all methodOop inline caches
  // 2. cleanup all nmethod inline caches
  // 3..deoptimized blocks with compiled code.

  // Clear caches
  lookupCache::flush();
  // Universe::cleanup_all_inline_caches();

  Universe::flush_inline_caches_in_methods();
  clear_inline_caches();

  // Convert all blocks with compiled code
  ConvertBlockClosure bl;
  Universe::object_iterate(&bl);

  FOR_ALL_NMETHODS(p) {
    debug_nm = p;
    if (p->isZombie()) {
      if (deoptimize)
        Processes::deoptimize_wrt(p);
      p->flush();
    }
  }
}

void zone::flushUnused() {
  // flush all nmethods marked as unused
  // NB: access methods are always unused since they don't do the LRU thing
  // chainFrames();
  FOR_ALL_NMETHODS(p) {
    // use makeZombie() for efficiency, not flush()
    // (see comment in zone::flush())
    p->makeZombie(true);
  }
  flushZombies();
  flushICache();
  // unchainFrames();
}

void zone::adjustPolicy() {
  // This routine does the policy decisions about flushing, using feedback
  // from previous flushes.  If the heap is too full, then we'll soon have
  // to compact again, leading to frequent pauses and hight overhead.  On
  // the other hand, if we flush too many nmethods before compacting, we'll
  // have high compilation overhead to recreate the flushed code.
# ifdef NOT_IMPLEMENTED
  if (DontUseAnyTimer) return;		// for reproducible results & debugging
  float timeSinceLast = cpuTimer->tickNo - compactTime;
  float overhead = compactDuration / timeSinceLast;
  if (overhead > COMPACT_OVERHEAD) {
    float currentFreeFrac = 1 - float(iZone->usedBytes()) / iZone->capacity();
    minFreeFrac = min(float(minFreeFrac * 1.5), float(minFreeFrac + 0.05));
    if (PrintCodeReclamation) {
      printf("(compact overhead %3.1f%%: increasing minFreeFrac to %3.1f%%) ",
	     overhead *100, minFreeFrac * 100);
    }
  } else if (overhead < COMPACT_OVERHEAD / 2) {
    minFreeFrac = max(float(minFreeFrac / 1.5), float(minFreeFrac - 0.05));
    if (PrintCodeReclamation) {
      printf("(compact overhead %3.1f%%: decreasing minFreeFrac to %3.1f%%) ",
	     overhead *100, minFreeFrac * 100);
    }
  }
  int minFree = int(iZone->capacity() * minFreeFrac);
  while (1) {
    int toFlush = minFree - (iZone->capacity() - iZone->usedBytes());
    if (toFlush <= 0) break;
    flushNextMethod(LRU_MAX_RECLAIMED);
  }
  compactTime = cpuTimer->tickNo;
# endif
}

# define LRU_MAX_RECLAIMED 250000 /* stop after having found this amount of */
# define LRUMaxVisited     150
void zone::doSweep() { sweeper(LRUMaxVisited, LRU_MAX_RECLAIMED); }

void zone::doWork() {
  if (needsSweep()) doSweep();
  if (needsCompaction()) compact();
}

void zone::compact(bool forced) {
  // BlockProfilerTicks bpt(exclude_nmethod_compact);
  // CSect cs(profilerSemaphore); // for profiler

  TraceTime t("*compacting nmethod cache...", PrintCodeReclamation);
  EventMarker em("compacting zone");
  
  // chainFrames();
  flushZombies();
  char* firstFree = NULL;
  if (!forced) adjustPolicy();
  if (needsCompaction()) {
    if (PrintCodeReclamation) std->print("I");
    firstFree = methodHeap->compact(moveInsts);
  }
  // unchainFrames();
  flushICache();
  
  verify_if_often();
  _needsCompaction = false;
  // compactDuration = cpuTimer->tickNo - compactTime;
}

void zone::free(nmethod* nm) {
  verify_if_often();
  if (LRUhand == nm) LRUhand = next_nm(nm);
  methodHeap->deallocate(nm, nm->size());
  verify_if_often();
}

void zone::addToCodeTable(nmethod* nm) {
  methodTable->add(nm);
}

void zone::clear_inline_caches() {
  TraceTime t("*flushing inline caches...", PrintInlineCacheInvalidation);
  EventMarker em("flushing inline caches");

  FOR_ALL_NMETHODS(p) {
    p->clear_inline_caches();
  }

  flushICache();
}

void zone::cleanup_inline_caches() {
  TraceTime t("*cleaning inline caches...", PrintInlineCacheInvalidation);
  EventMarker em("cleaning inline caches");

  FOR_ALL_NMETHODS(p) {
    p->cleanup_inline_caches();
  }

  flushICache();
}

inline int retirementAge(nmethod* n) {
  int delta = LRU_RETIREMENT_AGE;
  if (n->instsLen() <= LRU_CUTOFF)
    delta = max(delta, LRU_RETIREMENT_AGE * LRU_SMALL_BOOST);
  return n->lastUsed() + delta;
}

void zone::verify() {
  methodTable->verify();
  int n = 0;
  FOR_ALL_NMETHODS(p) {
    n++;
    p->verify();
  }
  if (n != numberOfNMethods())
    error("zone: inconsistent usedIDs value - should be %ld, is %ld",
	   n, numberOfNMethods());
  methodHeap->verify();
  jump_table()->verify();
}


void zone::switch_pointers(oop from, oop to) { 
  Unimplemented();
}

void zone::oops_do(void f(oop*)) {
  FOR_ALL_NMETHODS(nm) {
    nm->oops_do(f);
  }

  FOR_ALL_PICS(pic) {
    pic->oops_do(f);
  }
}

void zone::nmethods_do(void f(nmethod* nm)) {
  FOR_ALL_NMETHODS(p) {
    f(p);
  }
}

void zone::PICs_do(void f(PIC* pic)) {
  FOR_ALL_PICS(p) {
    f(p);
  }
}

# define NMLINE(format, n, ntot, ntot2) 				      \
  std->print(format, (n), 100.0 * (n) / (ntot), 100.0 * (n) / (ntot2))

class nmsizes {
  int n, insts, locs, scopes;
 public:
  nmsizes() {
    n = insts = locs = scopes = 0; }

  int total()   {
    return n * sizeof(nmethod)+ insts + locs + scopes; }

  bool isEmpty() { return n == 0; }

  void print(char* name, nmsizes& tot) {
    int bigTotal = tot.total();
    int myTotal = total();
    if (! isEmpty()) {
      std->print("%-13s (%ld methods): ", name, n);
      NMLINE("headers = %ld (%2.0f%%/%2.0f%%); ", n*sizeof(nmethod),
	     myTotal, bigTotal);
      NMLINE("insts = %ld (%2.0f%%/%2.0f%%);\n", insts, myTotal, bigTotal);
      NMLINE("\tlocs = %ld (%2.0f%%/%2.0f%%); ", locs, myTotal, bigTotal);
      NMLINE("scopes = %ld (%2.0f%%/%2.0f%%);\n", scopes, myTotal, bigTotal);
      NMLINE("\ttotal = %ld (%2.0f%%/%2.0f%%)\n", myTotal, myTotal, bigTotal);
    }
  }

  void print(char* title, int t) {
    std->print_cr("   %3d %s nmethods %2d%% = %4dK (hdr %2d%%, inst %2d%%, locs %2d%%, debug %2d%%)",
                  n,
		  title,
                  total() * 100 / t,
		  total() / K,
                  n * sizeof(nmethod) * 100 / total(),
                  insts               * 100 / total(),
                  locs                * 100 / total(),
                  scopes              * 100 / total());
  }

  void add(nmethod* nm) {
    n++;
    insts  += nm->instsLen();
    locs   += nm->locsLen();
    scopes += nm->scopes()->length();
  }
};

void zone::print() {
  nmsizes nms;
  nmsizes zombies;
  int uncommon = 0;

  FOR_ALL_NMETHODS(p) {
    if (p->isZombie()) {
      zombies.add(p);
    } else {
      nms.add(p);
      if (p->isUncommonRecompiled()) uncommon++;
    }
  }

  std->print_cr("Zone:");

  if (!nms.isEmpty()) {
    std->print_cr("  Code (%dK, %d%% used)", methodHeap->capacity() / K, (methodHeap->usedBytes() * 100) / methodHeap->capacity());
    nms.print("live", methodHeap->capacity());
  }
  if (uncommon) {
    std->print_cr("(%d live uncommon nmethods)", uncommon);
  }
  if (!zombies.isEmpty()) {
    zombies.print("dead", methodHeap->capacity());
    std->print_cr("  PICs (%dK, %d%% used)", picHeap->capacity() / K, (picHeap->usedBytes() * 100) / picHeap->capacity());
  }

  int n     = 0;
  int insts = 0;
  FOR_ALL_PICS(pic) {
    n++;
    insts += pic->code_size();
  }
  int total = insts + n * sizeof(PIC);

  if (n > 0) {
    std->print_cr("   %3d entries = %dK (hdr %2d%%, inst %2d%%)",
                  n,
   		  total / K,
                  n * sizeof(PIC) * 100 / total,
                  insts           * 100 / total);
  }
}

struct nm_hist_elem{
  nmethod* nm;
  int     count;
  int     size;
  int     sic_count;
  int     sic_size;
};

static int compareOop(const void *m1, const void *m2) {
  ResourceMark rm;
  struct nm_hist_elem *nmethod1 = (struct nm_hist_elem *) m1;
  struct nm_hist_elem *nmethod2 = (struct nm_hist_elem *) m2;
  return nmethod2->nm->method() - nmethod1->nm->method();
}

static int compareCount(const void *m1, const void *m2) {
  struct nm_hist_elem *nmethod1 = (struct nm_hist_elem *) m1;
  struct nm_hist_elem *nmethod2 = (struct nm_hist_elem *) m2;
  return nmethod2->count - nmethod1->count;
}

void zone::print_nmethod_histogram(int size) {
# ifdef NOT_IMPLEMENTED
  ResourceMark rm;
  nm_hist_elem* hist_array = NEW_RESOURCE_ARRAY(nm_hist_elem, numberOfNMethods());

  int*  compiled_nmethods = NEW_RESOURCE_ARRAY(int, numberOfNMethods());

  int n = 0;
  FOR_ALL_NMETHODS(p) {
    if (!p->isAccess() && !p->isZombie()) {
      hist_array[n].nm     = p;
      hist_array[n].size   = p->instsLen() +
                             p->locsLen() +
                             p->depsLen +
                             p->scopes->length();
      n++;
    }
  }

  qsort(hist_array, n, sizeof(nm_hist_elem), compareOop);
  
  int i = 0;
  for (i = 0; i < n; i++) compiled_nmethods[i] = 0;

  i = 0;
  int out = 0;
  while (i < n) {
    int counter     = 1;
    int all_size    = 0;
    int sic_counter = 0;
    int sic_size    = 0;

    oop method = hist_array[i].nm->method();
    if (hist_array[i].nm->flags.compiler == nm_new) {
      sic_counter++;
      sic_size += hist_array[i].size;
    }
    all_size = hist_array[i].size;

    i++;

    while (i < n && method == hist_array[i].nm->method()) {
      if (hist_array[i].nm->flags.compiler == nm_sic) {
	sic_counter++;
        sic_size += hist_array[i].size;
      }
      all_size += hist_array[i].size;
      counter++; i++;
    }
    compiled_nmethods[counter]++;
    if (counter > size) {
      hist_array[out]           = hist_array[i-1];
      hist_array[out].count     = counter;
      hist_array[out].size      = all_size;
      hist_array[out].sic_count = sic_counter;
      hist_array[out].sic_size  = sic_size;
      out++;
    }
  }

  qsort(hist_array, out, sizeof(nm_hist_elem), compareCount);

  printf("\n# nm \t # methods \t%% acc.\n");
  int nm_count = 0;
  for (i = 0; i < n; i++) {
    if (compiled_nmethods[i] > 0) {
      nm_count += i * compiled_nmethods[i];
      printf("%5ld \t%5ld \t\t%3ld %%\n", i, compiled_nmethods[i], 
	     (nm_count*100)/n);

    }
  }

  printf( "\nList of methods with more than %d nmethods compiled.\n", size);
  printf( " ALL(#,Kb)  Compiler(#,Kb) Method:\n");
  for (i = 0; i < out; i++) {
    printf("%4d,%-4d   %4d,%-4d ",
	   hist_array[i].count,     hist_array[i].size / 1024,
	   hist_array[i].sic_count, hist_array[i].sic_size / 1024);
    printName((methodKlass*) hist_array[i].nm->method()->klass(),
	       hist_array[i].nm->key.selector);
    printf("\n");
  }
  
  fflush(stdout);
# endif
}

bool zone::isDeltaPC(void* p) const {
  return methodHeap->contains(p)
      || picHeap->contains(p);
}

nmethod* zone::findNMethod(void* start) const {
  nmethod* n;
  if (methodHeap->contains(start)) {
    n = (nmethod*)methodHeap->findStartOfBlock(start);
    assert((char*)start < (char*)n->locsEnd(), "found wrong nmethod");
  }
  assert(methodHeap->contains(n), "not in zone");
  assert(n->isNMethod(), "findNMethod didn't find nmethod");
  assert(n->encompasses(start), "doesn't encompass start");
  return n;
}


nmethod* zone::findNMethod_maybe(void* start) const {
  // start *may* point into the instructions part of a nmethod; find it
  if (!methodHeap->contains(start)) return NULL;
  // relies on FOR_ALL_NMETHODS to enumerate in ascending order
  FOR_ALL_NMETHODS(p) {
    if (p->insts() > (char*)start) return NULL;
    if (p->instsEnd() > (char*)start) return p;
  }
  return NULL;
}

void zone::mark_dependents_for_deoptimization() {
  ResourceMark rm;
  FOR_ALL_NMETHODS(nm) {
    if (nm->depends_on_invalid_klass()) {
      GrowableArray<nmethod*>* nms = nm->invalidation_family();
      for (int index = 0; index < nms->length(); index++) {
        nmethod* elem = nms->at(index);	
	if (TraceApplyChange) {
          std->print("invalidating "); 
          elem->print_value_on(std);
          std->cr();
        }
	elem->mark_for_deoptimization();
      }
    }
  }
}

void zone::mark_all_for_deoptimization() {
  FOR_ALL_NMETHODS(nm) {
    nm->mark_for_deoptimization();
  }
}

void zone::unmark_all_for_deoptimization() {
  FOR_ALL_NMETHODS(nm) nm->unmark_for_deoptimization();
}

void zone::make_marked_nmethods_zombies() {
  FOR_ALL_NMETHODS(nm) {
    if (nm->is_marked_for_deoptimization()) {
      nm->makeZombie(true);
    }
  }
}

char* zone::instsStart() { return methodHeap->startAddr(); };
int zone::instsSize()	 { return methodHeap->capacity(); }

inline nmethod* zone::next_circular_nm(nmethod* nm) {
  nm = next_nm(nm);
  if (nm == NULL) nm = first_nm();
  return nm;
}

// called every LRU_RESOLUTION seconds or by reclaimNMethods if needed
// returns time at which oldest non-reclaimed nmethod will be reclaimed
int zone::sweeper(int maxVisit, int maxReclaim,
		    int* nvisited, int* nbytesReclaimed) {
# ifdef NOT_IMPLEMENTED
  EventMarker em("LRU sweep");
  ResourceMark rm;

  timer tmr;   
  int visited = 0;
  int nused = 0;
  int nbytes = 0;
  int nextTime = LRUtime + LRU_RETIREMENT_AGE;
  nmethod* first = first_nm();
  if (! first) return -1;
  
  nmethod* p = LRUhand ? LRUhand : first;
  if (PrintLRUSweep || PrintLRUSweep2) {
    printf("*starting LRU sweep...");
    fflush(stdout);
    tmr.start();
  }
    
  do {
    if (PrintLRUSweep2) printf("\n*inspecting %#lx (id %ld): ", p, p->id);
    
    if ((p->isZombie() ||
	 p->isDebug()  ||
	 p->codeTableLink.isEmpty() && !p->isUncommon()) &&
	p->frame_chain == NULL) {
      // can be flushed - nobody will ever use it again
      if (PrintLRUSweep2) printf(" %s; flushed",
				 p->isZombie() ? "zombie" :
				 (p->isDebug() ? "debug" : "unreachable"));
      nbytes += iZone->sizeOfBlock(p);
      p->flush();
    } else if (p->isUsed()) {
      // has been used
      nused++;
      if (PrintLRUSweep2) {
	printf("used");
	int diff = useCount[p->id] - p->oldCount;
	if (diff) printf(" %ld times", diff);
      }
      if (LRUDecayFactor > 1) {
	useCount[p->id] = int(useCount[p->id] / LRUDecayFactor);
      }
      p->oldCount = useCount[p->id];
      LRUtable[p->id].unused = true;
      LRUtable[p->id].lastUsed = LRUtime;
      if (p->zoneLink.notEmpty()) {
	if (PrintLRUSweep2) printf("; removed from replCandidates");
	p->zoneLink.remove();		// no longer a replacement candidate
	nbytes -= iZone->sizeOfBlock(p);
      }
    } else if (retirementAge(p) < LRUtime && p->zoneLink.isEmpty()) {
      if (PrintLRUSweep2) printf("added to replCandidates");
      replCandidates.add(&p->zoneLink);
      nbytes += iZone->sizeOfBlock(p);
    } else {
      int age = retirementAge(p);
      if (age < nextTime) {
	nextTime = age;
      }
      if (PrintLRUSweep2) {
	printf("unused (age %ld)", LRUtime - p->lastUsed());
	if (p->zoneLink.notEmpty())
	  printf(" already scheduled for replacement");
      }
    }
    
    nmethod* oldp = p;
    p = next_circular_nm(p);
    if (p < oldp) {				// wrap around
      LRUtime++;
      // The LRU scheme will actually fail if LRUtime > 2^16, but that
      // won't happen very often (every 20*LRU_RESOLUTION CPU hours).
      if (PrintLRUSweep2) printf("\n*new LRU time: %ld", LRUtime);
    }
  } while (++visited < maxVisit && nbytes < maxReclaim && p);
  
  if (needsSweep() && LRUDecayFactor > 1) {
    // called from timer; decay count stubs
    stubs->decay(LRUDecayFactor);
  }
  
  LRUhand = p;
  _needsSweep = false;
  if (PrintLRUSweep || PrintLRUSweep2) {
    printf(" done: %ld/%ld visits, %ld bytes, %ld ms.\n",
	   nused, visited, nbytes, tmr.time());
    fflush(stdout);
  }
  if (nvisited) *nvisited = visited;
  if (nbytesReclaimed) *nbytesReclaimed = nbytes;
  return nextTime;
# endif
  return 0;
}

int zone::LRU_time() { return LRUtime; }

void printAllNMethods() {
  for(nmethod* m = Universe::code->first_nm(); m != NULL; m = Universe::code->next_nm(m)) {
    if (!m->isZombie()) {
      m->scopes()->print_partition();
    }
  }
}

#endif

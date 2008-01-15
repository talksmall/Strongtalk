/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.20 $ */
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
# include "incls/_dispatchTable.cpp.incl"

//
// The following defines the dispatch table for the interpreter.
// - inserted by Lars to please the Microsoft VC++ 2.0 linker.
//
// %note: A link error will occur if a conflict exists between
//        the dispatch table and the interpreter.
//

extern "C" void illegal();
//extern "C" void single_step_stub();

#define L(num) interpreter_case_##num
#define M(num) &interpreter_case_##num
#define D(num) extern "C" void L(num)##();
#define _(num) extern "C" void L(num)##() { Interpreter::illegal(); }

#define TEST_GENERATION_

#if(!defined(TEST_GENERATION_))

//0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F
_(0)   _(1)   _(2)   _(3)   _(4)   _(5)   _(6)   _(7)   _(8)   _(9)   _(10)  _(11)  _(12)  _(13)  _(14)  _(15)  // 0
_(16)  _(17)  _(18)  _(19)  _(20)  _(21)  _(22)  _(23)  _(24)  _(25)  _(26)  _(27)  _(28)  _(29)  _(30)  _(31)  // 1
_(32)  _(33)  _(34)  _(35)  _(36)  _(37)  _(38)  _(39)  _(40)  _(41)  _(42)  _(43)  _(44)  _(45)  _(46)  _(47)  // 2
_(48)  _(49)  _(50)  _(51)  _(52)  _(53)  _(54)  _(55)  _(56)  _(57)  _(58)  _(59)  _(60)  _(61)  _(62)  _(63)  // 3
_(64)  _(65)  _(66)  D(67)  D(68)  D(69)  D(70)  D(71)  _(72)  _(73)  _(74)  _(75)  _(76)  _(77)  _(78)  _(79)  // 4
_(80)  _(81)  _(82)  D(83)  D(84)  D(85)  D(86)  D(87)  _(88)  _(89)  _(90)  _(91)  _(92)  _(93)  _(94)  _(95)  // 5
D(96)  D(97)  D(98)  D(99)  D(100) D(101) D(102) D(103) _(104) D(105) D(106) D(107) D(108) D(109) D(110) D(111) // 6
D(112) D(113) D(114) D(115) D(116) D(117) D(118) D(119) D(120) D(121) D(122) D(123) D(124) D(125) D(126) D(127) // 7
_(128) _(129) _(130) _(131) _(132) _(133) _(134) _(135) _(136) _(137) _(138) _(139) D(140) D(141) D(142) D(143) // 8
_(144) _(145) _(146) _(147) _(148) _(149) _(150) _(151) _(152) _(153) _(154) _(155) D(156) D(157) D(158) D(159) // 9
_(160) _(161) _(162) _(163) _(164) _(165) _(166) _(167) _(168) _(169) _(170) _(171) D(172) D(173) _(174) _(175) // A
_(176) D(177) _(178) D(179) _(180) _(181) _(182) _(183) D(184) D(185) _(186) _(187) _(188) _(189) _(190) _(191) // B
_(192) D(193) _(194) D(195) _(196) _(197) _(198) _(199) D(200) D(201) _(202) _(203) _(204) _(205) _(206) _(207) // C
_(208) _(209) _(210) _(211) _(212) _(213) _(214) _(215) _(216) _(217) _(218) _(219) _(220) _(221) _(222) _(223) // D
_(224) _(225) _(226) _(227) _(228) _(229) _(230) _(231) _(232) _(233) _(234) _(235) _(236) _(237) D(238) D(239) // E
_(240) _(241) _(242) _(243) _(244) _(245) _(246) _(247) _(248) _(249) _(250) _(251) _(252) _(253) _(254) _(255) // F

#else

#ifdef JUNK // we don't need it anymore anyway

//0      1      2      3      4      5      6      7      8      9      A      B      C      _      E      F
_(0)   _(1)   _(2)   _(3)   _(4)   _(5)   _(6)   _(7)   _(8)   _(9)   _(10)  _(11)  _(12)  _(13)  _(14)  _(15)  // 0
_(16)  _(17)  _(18)  _(19)  _(20)  _(21)  _(22)  _(23)  _(24)  _(25)  _(26)  _(27)  _(28)  _(29)  _(30)  _(31)  // 1
_(32)  _(33)  _(34)  _(35)  _(36)  _(37)  _(38)  _(39)  _(40)  _(41)  _(42)  _(43)  _(44)  _(45)  _(46)  _(47)  // 2
_(48)  _(49)  _(50)  _(51)  _(52)  _(53)  _(54)  _(55)  _(56)  _(57)  _(58)  _(59)  _(60)  _(61)  _(62)  _(63)  // 3
_(64)  _(65)  _(66)  _(67)  _(68)  _(69)  _(70)  _(71)  _(72)  _(73)  _(74)  _(75)  _(76)  _(77)  _(78)  _(79)  // 4
_(80)  _(81)  _(82)  _(83)  _(84)  _(85)  _(86)  _(87)  _(88)  _(89)  _(90)  _(91)  _(92)  _(93)  _(94)  _(95)  // 5
_(96)  _(97)  _(98)  _(99)  _(100) _(101) _(102) _(103) _(104) _(105) _(106) _(107) _(108) _(109) _(110) _(111) // 6
_(112) _(113) _(114) _(115) _(116) _(117) _(118) _(119) _(120) _(121) _(122) _(123) _(124) _(125) _(126) _(127) // 7
_(128) _(129) _(130) _(131) _(132) _(133) _(134) _(135) _(136) _(137) _(138) _(139) _(140) _(141) _(142) _(143) // 8
_(144) _(145) _(146) _(147) _(148) _(149) _(150) _(151) _(152) _(153) _(154) _(155) _(156) _(157) _(158) _(159) // 9
_(160) _(161) _(162) _(163) _(164) _(165) _(166) _(167) _(168) _(169) _(170) _(171) _(172) _(173) _(174) _(175) // A
_(176) _(177) _(178) _(179) _(180) _(181) _(182) _(183) _(184) _(185) _(186) _(187) _(188) _(189) _(190) _(191) // B
_(192) _(193) _(194) _(195) _(196) _(197) _(198) _(199) _(200) _(201) _(202) _(203) _(204) _(205) _(206) _(207) // C
_(208) _(209) _(210) _(211) _(212) _(213) _(214) _(215) _(216) _(217) _(218) _(219) _(220) _(221) _(222) _(223) // _
_(224) _(225) _(226) _(227) _(228) _(229) _(230) _(231) _(232) _(233) _(234) _(235) _(236) _(237) _(238) _(239) // E
_(240) _(241) _(242) _(243) _(244) _(245) _(246) _(247) _(248) _(249) _(250) _(251) _(252) _(253) _(254) _(255) // F

#endif

#endif // TEST_GENERATION

// The dispatch table holds the dispatch addresses currently used (i.e., individual entries might be
// patched with a stub-routine, etc.). The original table holds the untouched original entry points.
//
// Note: At the time beeing, the original tale is modified via Bytecodes::set_entry_point(...) for
//       the generated bytecodes.

extern "C" doFn dispatch_table[Bytecodes::number_of_codes];
extern "C" doFn original_table[Bytecodes::number_of_codes];

doFn dispatch_table[Bytecodes::number_of_codes];
doFn original_table[Bytecodes::number_of_codes];

#ifdef JUNK

 = {
  M(0),  M(1),  M(2),  M(3),  M(4),  M(5),  M(6),  M(7),  M(8),  M(9),  M(10), M(11), M(12), M(13), M(14), M(15),  // 0
  M(16), M(17), M(18), M(19), M(20), M(21), M(22), M(23), M(24), M(25), M(26), M(27), M(28), M(29), M(30), M(31),  // 1
  M(32), M(33), M(34), M(35), M(36), M(37), M(38), M(39), M(40), M(41), M(42), M(43), M(44), M(45), M(46), M(47),  // 2
  M(48), M(49), M(50), M(51), M(52), M(53), M(54), M(55), M(56), M(57), M(58), M(59), M(60), M(61), M(62), M(63),  // 3
  M(64), M(65), M(66), M(67), M(68), M(69), M(70), M(71), M(72), M(73), M(74), M(75), M(76), M(77), M(78), M(79),  // 4
  M(80), M(81), M(82), M(83), M(84), M(85), M(86), M(87), M(88), M(89), M(90), M(91), M(92), M(93), M(94), M(95),  // 5
  M(96), M(97), M(98), M(99), M(100),M(101),M(102),M(103),M(104),M(105),M(106),M(107),M(108),M(109),M(110),M(111), // 6
  M(112),M(113),M(114),M(115),M(116),M(117),M(118),M(119),M(120),M(121),M(122),M(123),M(124),M(125),M(126),M(127), // 7
  M(128),M(129),M(130),M(131),M(132),M(133),M(134),M(135),M(136),M(137),M(138),M(139),M(140),M(141),M(142),M(143), // 8
  M(144),M(145),M(146),M(147),M(148),M(149),M(150),M(151),M(152),M(153),M(154),M(155),M(156),M(157),M(158),M(159), // 9
  M(160),M(161),M(162),M(163),M(164),M(165),M(166),M(167),M(168),M(169),M(170),M(171),M(172),M(173),M(174),M(175), // A
  M(176),M(177),M(178),M(179),M(180),M(181),M(182),M(183),M(184),M(185),M(186),M(187),M(188),M(189),M(190),M(191), // B
  M(192),M(193),M(194),M(195),M(196),M(197),M(198),M(199),M(200),M(201),M(202),M(203),M(204),M(205),M(206),M(207), // C
  M(208),M(209),M(210),M(211),M(212),M(213),M(214),M(215),M(216),M(217),M(218),M(219),M(220),M(221),M(222),M(223), // D
  M(224),M(225),M(226),M(227),M(228),M(229),M(230),M(231),M(232),M(233),M(234),M(235),M(236),M(237),M(238),M(239), // E
  M(240),M(241),M(242),M(243),M(244),M(245),M(246),M(247),M(248),M(249),M(250),M(251),M(252),M(253),M(254),M(255)  // F
};

#endif

extern "C" int* frame_breakpoint = (int*) -1;

dispatchTable::Mode dispatchTable::mode;


u_char** dispatchTable::table() {
  return (u_char**)&dispatch_table[0];
}


void dispatchTable::reset() {
  for (int index = 0; index < Bytecodes::number_of_codes; index++) {
    dispatch_table[index] = original_table[index];
  }
  mode = normal_mode;
}


void dispatchTable::patch_with_sst_stub() {
  for (int index = 0; index < Bytecodes::number_of_codes; index++) {
    if (Bytecodes::single_step(Bytecodes::Code(index))) {
      dispatch_table[index] = (doFn)StubRoutines::single_step_stub();
    } else {
      dispatch_table[index] = original_table[index];
    }
  }
}


void dispatchTable::intercept_for_step() {
  if (!in_step_mode()) {
    patch_with_sst_stub();
    mode = step_mode;
//slr mod - the original value depends on the memory addresses of method bytecodes 
//    being < 0x80000000 as in 32 bit Windows
//    frame_breakpoint = (int*) -1;
    frame_breakpoint = (int*) 0x80000000;
//end slr mod
  }
}


void dispatchTable::intercept_for_next(int* fr) {
  frame_breakpoint = fr;
  if (!in_next_mode()) {
    patch_with_sst_stub();
    mode = next_mode;
  }
}


static int return_codes_size = 12;
static Bytecodes::Code return_codes[] = {
  Bytecodes::return_tos_pop_0,
  Bytecodes::return_tos_pop_1,
  Bytecodes::return_tos_pop_2,
  Bytecodes::return_tos_pop_n,

  Bytecodes::return_self_pop_0,
  Bytecodes::return_self_pop_1,
  Bytecodes::return_self_pop_2,
  Bytecodes::return_self_pop_n,

  Bytecodes::return_tos_zap_pop_n,
  Bytecodes::return_self_zap_pop_n,
  Bytecodes::non_local_return_tos_pop_n,
  Bytecodes::non_local_return_self_pop_n
};


void dispatchTable::intercept_for_return(int* fr)  {
  frame_breakpoint = fr;
  if (!in_return_mode()) {
    reset();
    for (int index = 0; index < return_codes_size; index++) {
      Bytecodes::Code code = return_codes[index];
      if (Bytecodes::single_step(code)) {
        dispatch_table[code] = (doFn)StubRoutines::single_step_stub();
      } else {
        dispatch_table[index] = original_table[index];
      }
    }
    mode = next_mode;
  }
}


void dispatchTable_init() {
  dispatchTable::reset();
}


void intercept_for_single_step() {
  dispatchTable::intercept_for_step();
}


void print_dt() {
  for (int i = 0; i < 255; i++) {
    printf("0x%02x: 0x%08x\n", i, ((int*)dispatchTable::table())[i]);
  }
  printf("\n");
}

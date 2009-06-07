/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.3 $ */
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


class memConverter;
class ClassChange;

class Reflection : AllStatic {
 private:
  // Variables used for schema change
  static GrowableArray<ClassChange*> *class_changes; // Class changes
  static GrowableArray<memOop>       *converted;     // Converted objects

  static int invocations_offset() { return 3; }
 private:
  // registers all changes in 'class_changes'
  static void register_class_changes(mixinOop new_mixin, objArrayOop invocations);

  // sets or resets the invalid bit in the header for old_klass in class_changes
  static void invalidate_classes(bool value);

  // find the change structure for a given class
  static ClassChange* find_change_for(klassOop klass);

  // computes if a schema change is necessary
  static bool needs_schema_change();

  // FOR NO SCHEMA CHANFES
  static void update_classes(bool class_vars_changed, bool instance_methods_changed, bool class_methods_changed);

  // FOR SCHEMA CHANGES
 
  // builds the new classes and converters
  static void setup_schema_change();

  static void update_class(klassOop klass,
			   mixinOop new_mixin, 
			   mixinOop old_mixin,
			   bool     instance_methods_changed,
			   bool     class_methods_changed,
			   bool     class_vars_changed);


  static bool has_methods_changed(mixinOop new_mixin, mixinOop old_mixin);
  static bool has_class_vars_changed(mixinOop new_mixin, mixinOop old_mixin);

  static void apply_change(mixinOop    new_mixin,
			  mixinOop    old_mixin,
		          objArrayOop invocations);

  static memOop convert_object(memOop obj);

 public:
  // Entry for primitiveApplyChange:ifFail:
  static oop apply_change(objArrayOop change);

  // Converts an object if necessary (used when scanning the object heap)
  static void convert(oop* p);

  // place forward pointer and 
  static void forward(memOop old_obj, memOop new_obj);
};

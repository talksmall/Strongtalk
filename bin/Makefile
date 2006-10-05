# Copyright 1994, LongView Technologies L.L.C. $Revision: 1.24 $
# Copyright (c) 2006, Sun Microsystems, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, 
#   this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice, 
#   this list of conditions and the following  disclaimer in the documentation
#   and/or other materials provided with the distribution.
# * Neither the name of Sun Microsystems nor the names of its contributors 
#   may be used to endorse or promote products derived from this software 
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
# POSSIBILITY OF SUCH DAMAGE


#SHELL = /bin/csh -f

DB1 = ../vm/deps/includeDB
DB2 = ../vm/deps/includeDB2

update: ${DB1} ${DB2}
	@cat ${DB1} ${DB2} > includeDB.all
	@../tools/makedeps includeDB.current includeDB.all
	@mv includeDB.all includeDB.current

lists: ${DB1} ${DB2}
	@rm -rf incls
	@mkdir incls
	@cat ${DB1} ${DB2} > includeDB.all
	@../tools/makedeps includeDB.all
	@mv includeDB.all includeDB.current

numbers:
	wc ../vm/*/*.hpp ../vm/*/*.cpp ../vm/*/*.asm

clean:	# remove all build & execution related files
	rm -f includeDB.*
	rm -f *.ncb
	rm -rf incls
	rm -rf debug
	rm -rf fast
	rm -rf product
	rm -f ../*.log
	rm -f ../*.log.old
	rm -f ../*.ilk
	rm -f ../*.bak

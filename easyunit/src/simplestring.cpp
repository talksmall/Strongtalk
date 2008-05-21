/*
EasyUnit : Simple C++ Unit testing framework
Copyright (C) 2004 Barthelemy Dagenais

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Barthelemy Dagenais
barthelemy@prologique.com
*/

#include "simplestring.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace easyunit;

static const int DEFAULT_SIZE = 20;

SimpleString::SimpleString ()
: buffer((char*) malloc(sizeof(char)))
{
	buffer [0] = '\0';
}

SimpleString::SimpleString (const char *otherBuffer)
: buffer (NEW_BUFFER(strlen (otherBuffer) + 1))
{
	strcpy (buffer, otherBuffer);
}

SimpleString::SimpleString (const SimpleString& other)
{
	buffer = NEW_BUFFER(other.size() + 1);
	strcpy(buffer, other.buffer);
}


SimpleString SimpleString::operator= (const SimpleString& other)
{
	delete buffer;
	buffer = NEW_BUFFER(other.size() + 1);
	strcpy(buffer, other.buffer);	
	return *this;
}

SimpleString SimpleString::operator+ (const SimpleString& other)
{
	SimpleString newS;
	free(newS.buffer);
	newS.buffer = NEW_BUFFER(this->size()+other.size()+1);
	strcpy(newS.buffer,this->asCharString());
	newS.buffer= strcat(newS.buffer,other.asCharString());
	return newS;
}

char *SimpleString::asCharString () const
{
	return buffer;
}

int SimpleString::size() const
{
	return strlen (buffer);
}

SimpleString::~SimpleString ()
{
	free(buffer);
}

bool easyunit::operator== (const SimpleString& left, const SimpleString& right)
{
	return !strcmp (left.asCharString (), right.asCharString ());
}

bool easyunit::operator!= (const SimpleString& left, const SimpleString& right)
{
	return !(left == right);
}

SimpleString easyunit::StringFrom (bool value)
{
	char buffer [sizeof ("false") + 1];
	sprintf (buffer, "%s", value ? "true" : "false");
	return SimpleString(buffer);
}

SimpleString easyunit::StringFrom (const char *value)
{
	return SimpleString(value);
}

SimpleString easyunit::StringFrom (long value)
{
	char buffer [DEFAULT_SIZE];
	sprintf (buffer, "%ld", value);

	return SimpleString(buffer);
}

SimpleString easyunit::StringFrom (int value)
{
	char buffer [DEFAULT_SIZE];
	sprintf (buffer, "%d", value);

	return SimpleString(buffer);
}

SimpleString easyunit::StringFrom (double value)
{
	char buffer [DEFAULT_SIZE];
	sprintf (buffer, "%lf", value);

	return SimpleString(buffer);
}

SimpleString easyunit::StringFrom (const SimpleString& value)
{
	return SimpleString(value);
}




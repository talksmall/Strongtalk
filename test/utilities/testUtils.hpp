
#define as_large_integer(value) \
  byteArrayPrimitives::largeIntegerFromSmallInteger(as_smiOop(value), klassOop(Universe::find_global("LargeInteger")))

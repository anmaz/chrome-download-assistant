#ifndef UTIL_H_
#define UTIL_H_

#include "npfunctions.h"

namespace utils {
  class IdentifiertoString {
  public:
    explicit IdentifiertoString(NPIdentifier identifier)
      : identifier_name_(NULL) {
        identifier_name_ = NPN_UTF8FromIdentifier(identifier);
    }
    const char* identifier_name() const { return identifier_name_; }
    operator const char*() const { return identifier_name_; }
    ~IdentifiertoString() { if (identifier_name_) NPN_MemFree(identifier_name_); }

  private:
    // Disable evil constructors.
    IdentifiertoString();
    IdentifiertoString(const IdentifiertoString&);

    char* identifier_name_;
  };
}

#endif
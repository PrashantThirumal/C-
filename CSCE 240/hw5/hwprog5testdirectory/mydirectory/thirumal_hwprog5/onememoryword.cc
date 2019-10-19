#include "onememoryword.h"

/***************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456
 * Class 'OneMemoryWord' as a container for one word of memory.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Used with permission and modified by: Prashant Thirumal
 * Date: 12 November 2018
 *
 * This exists almost entirely to allow the text of a function to indicate
 * what one is extracting from the 16 bit pattern that is a memory word,
 * and to format the bits for printing as 3 + 1 + 12 for opcode, indirect,
 * and address.
**/

/***************************************************************************
 * Constructor
**/
OneMemoryWord::OneMemoryWord() {
}

/***************************************************************************
 * Constructor
**/
OneMemoryWord::OneMemoryWord(string thestring) {
  Initialize(thestring);
}

/***************************************************************************
 * Destructor
**/
OneMemoryWord::~OneMemoryWord() {
}

/***************************************************************************
 * Accessors and Mutators
**/

/***************************************************************************
 * Accessor for the 'address_bits_'.
**/
string OneMemoryWord::GetAddressBits() const {
  return bit_pattern_.substr(4);
}

/***************************************************************************
 * Accessor for the 'bit_pattern_'.
**/
string OneMemoryWord::GetBitPattern() const {
  return bit_pattern_;
}

/***************************************************************************
 * Accessor for the 'indirect_flag_'.
**/
string OneMemoryWord::GetIndirectFlag() const {
  return bit_pattern_.substr(3, 1);
}

/***************************************************************************
 * Accessor for the 'mnemonic_bits_'.
**/
string OneMemoryWord::GetMnemonicBits() const {
  return bit_pattern_.substr(0, 3);
}

/***************************************************************************
 * Accessor for the 'mnemonic_code_'.
string OneMemoryWord::GetMnemonicCode() const {
  string codebits = this->GetMnemonicBits();
  string mnemonic_code = DABnamespace::GetMnemonicFromBits(codebits);
  return mnemonic_code;
}
**/

/***************************************************************************
 * Mutator for the 'bit_pattern_'.
**/
void OneMemoryWord::SetBitPattern(string what) {
  bit_pattern_ = what;
}

/***************************************************************************
 * General functions.
**/

/***************************************************************************
 * Function 'Initialize'.
**/
void OneMemoryWord::Initialize(string bit_pattern) {
  bit_pattern_ = bit_pattern;
}

/***************************************************************************
 * Function 'ToString'.
 * This function formats a 'OneMemoryWord' for prettyprinting.
 * This makes sense if the word is an instruction.  If it's just a number or a
 * bit pattern, then the prettyprinting makes less sense.  But we do the
 * prettyprinting because taking out spaces in reading a line is easier than
 * adding in spaces that would separate mnemonic, etc.
 *
 * Returns:
 *   the prettyprint string for printing
**/
string OneMemoryWord::ToString() const {
#ifdef EBUG
  Utils::log_stream << "enter ToString" << endl;
#endif
  string sss = "";
  sss += this->GetMnemonicBits() + " " + this->GetIndirectFlag() + " " +
         this->GetAddressBits();


//  s += Utils::Format(linecounter_, 5) + " ";

//  s += Utils::Format(pc_, 4) + "  ";
//  s += globals_.DecToBitString(pc_, 12) + " ";

/*
  if (code_ == "nullcode") {
    s += Utils::Format("xxxx xxxx xxxx xxxx", 19);
  } else {
    s = s       + Utils::Format((code_).substr( 0, 4), 4)
          + " " + Utils::Format((code_).substr( 4, 4), 4)
          + " " + Utils::Format((code_).substr( 8, 4), 4)
          + " " + Utils::Format((code_).substr(12, 4), 4);
  }

  if (label_ == "nulllabel") {
    s += " " + Utils::Format("...", 3);
  } else {
    s += " " + Utils::Format(label_, 3);
  }

  if (mnemonic_ == "nullmnemonic") {
    s += " " + Utils::Format("...", 3);
  } else {
    s += " " + Utils::Format(mnemonic_, 6);
  }

  if (addr_ == "*") {
    s += " " + Utils::Format("*", 1);
  } else {
    s += " " + Utils::Format(" ", 1);
  }

  if (symoperand_ == "nullsymoperand") {
    s += " " + Utils::Format("...", 3);
  } else {
    s += " " + Utils::Format(symoperand_, 3);
  }

  if (hex_.IsNotNull()) {
    s += " " + Utils::Format(".....", 5);
  } else {
    s += " " + hex_.ToString();
  }

  if (comments_ != "nullcomments") {
    if (is_all_comment_) {
      s = Utils::Format(linecounter_, 5) + " ";
      s += Utils::Format(" ", 41);
      s += " " + comments_;
    } else {
      s += " " + comments_;
    }
  }

  if ((error_messages_.length() > 0) && (error_messages_ != "nullerrormessages")) {
    s += error_messages_;
  }
*/

#ifdef EBUG
  Utils::log_stream << "leave ToString" << endl;
#endif
  return sss;
}

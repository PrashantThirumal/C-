#include "pullet16interpreter.h"

/***************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456
 * Class 'Interpreter' for interpreting the Pullet16 code.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Used with permission and modified by: Prashant Thirumal
 * Date: 2 October 2018
 *
 * This class will do the interpretation of ASCII versions of 'executable'
 * files for the Pullet16.  At the moment, for Homework 3, it simply
 * reads into memory the input and dumps a formatted version of that
 * to an output file.
 *
 * Documentation for details of the functions is done as headers for
 * the functions.
**/

/***************************************************************************
 * Constructor
**/
Interpreter::Interpreter() {
}

/***************************************************************************
 * Destructor
**/
Interpreter::~Interpreter() {
}

/***************************************************************************
 * Accessors and Mutators
**/

/***************************************************************************
 * General functions.
**/

/******************************************************************************
 * Function 'Decode'.
 * This top level function decodes the ASCII of the machine code into an
 *   assembly language mnemonic plus etc.
 *
 * Note that if the machine code is just a number, the decoding will be bogus.
 *
 * Parameters:
 *   the_ascii - the ASCII to decode
 *
 * Returns:
 *   a string version of the assembly language
**/
string Interpreter::Decode(string the_ascii) {
#ifdef EBUG
  Utils::log_stream << "enter Decode" << endl;
#endif

  // your code goes here
  // Variable that is returned which contains the code fully decoded;

  string return_value;

  // Creating a map that matches each opcode to its respective mnemonic

  code_to_mnemonic_["000"] = "BAN";
  code_to_mnemonic_["001"] = "SUB";
  code_to_mnemonic_["010"] = "STC";
  code_to_mnemonic_["011"] = "AND";
  code_to_mnemonic_["100"] = "ADD";
  code_to_mnemonic_["101"] = "LD ";
  code_to_mnemonic_["110"] = "BR ";
  code_to_mnemonic_["111"] = "CHECK LAST FOUR";

  // Sieve out the bits that need to be decoded
  // initialize the respective decoded strings
  // Last twelve bits decoded returns a decimal number which is an int

  string first_three = the_ascii.substr(0, 3);
  string fourth = the_ascii.substr(3, 1);
  string last_twelve = the_ascii.substr(4, 12);
  string first_three_decoded = " ";
  string fourth_decoded = " ";
  int last_twelve_decoded = 0;

  // Decode the first bits
  // If the first 3 bits are 111 then look at the last 4 bits

  first_three_decoded = code_to_mnemonic_[first_three];

  if ( first_three_decoded == "CHECK LAST FOUR" ) {
    string last_four = the_ascii.substr(12, 4);

    if ( last_four == "0001" ) {
      first_three_decoded = "RD";
    } else if ( last_four == "0010" ) {
      first_three_decoded = "STP";
    } else if ( last_four == "0011" ) {
      first_three_decoded = "WRT";
    } else { first_three_decoded = "XXX"; }
  }

  // Decode the fourth bit

  if ( fourth == "0" ) {
    fourth_decoded = " ";
  }
  if ( fourth == "1" ) {
    fourth_decoded = "*";
  }

  // Convert the last twelve bits to decimal and convert it to a string

  last_twelve_decoded = std::stoi(last_twelve, nullptr, 2);
  string twelve_decoded = std::to_string(last_twelve_decoded);

  // Reset all the values if the opcode is not valid

  if ( first_three_decoded == "XXX" ) {
    fourth_decoded = "X";
    last_twelve = "XXXXXXXXXXXX";
  }

  return_value = " CODE  " + first_three_decoded + "  " + fourth_decoded
                           + "  " + last_twelve + "  " + twelve_decoded;

#ifdef EBUG
  Utils::log_stream << "leave Decode" << endl;
#endif

  return return_value;
}

/******************************************************************************
 * Function 'DumpProgram'.
 * This top level function dumps the ASCII of the machine code from memory.
 *
 * Parameters:
 *   out_stream - the scanner to read for source code
**/
void Interpreter::DumpProgram(ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter DumpProgram" << endl;
#endif

  // for loop to print out the source code from vector

  for ( unsigned int i = 0; i < memory_.size(); ++i ) {
    out_stream << "MEMORY " << memory_.at(i) << " "
               << Decode(memory_.at(i)) << endl;
  }

#ifdef EBUG
  Utils::log_stream << "leave DumpProgram" << endl;
#endif
}

/******************************************************************************
 * Function 'ReadProgram'.
 * This top level function reads the ASCII of the machine code into memory.
 *
 * Parameters:
 *   in_scanner - the scanner to read for source code
**/
void Interpreter::ReadProgram(Scanner& in_scanner) {
#ifdef EBUG
  Utils::log_stream << "enter ReadProgram" << endl;
#endif

  // add all the lines in the source code to the vector

  while ( in_scanner.HasNext() ) {
    string inputstring = in_scanner.NextLine();
    memory_.push_back(inputstring);
  }


#ifdef EBUG
  Utils::log_stream << "leave ReadProgram" << endl;
#endif
}

/******************************************************************************
 * Function 'ToString'.
 *
 * This outputs in the prettyprinted string:
 *   PC, the program counter
 *   ACC, the accumulator, as an integer and as a bitstring
 *   a dump of memory from 0 through the max memory for this program
 *
 * Returns:
 *   the prettyprint string for printing
**/
string Interpreter::ToString() {
#ifdef EBUG
  Utils::log_stream << "enter ToString" << endl;
#endif

  string s = "";

  s += "PC    " + Utils::Format(pc_, 8) + "\n";

//  int twoscomplement = this->TwosComplementInteger(accum_);
//  s += "ACCUM " + Utils::Format(twoscomplement, 8)
//                + " " + DABnamespace::DecToBitString(accum_, 16)
//                + "\n\n";

  int memorysize = memory_.size();
  for (int outersub = 0; outersub < memorysize; outersub += 4) {
    s += "MEM " + Utils::Format(outersub, 4)
                + "-"
                + Utils::Format(outersub+3, 4);
    for (int innersub = outersub; innersub < outersub + 4; ++innersub) {
      if (innersub < memorysize) {
        s += " " + memory_.at(innersub);
      }
    }
    s += "\n";
  }

#ifdef EBUG
  Utils::log_stream << "leave ToString" << endl;
#endif

  return s;
}

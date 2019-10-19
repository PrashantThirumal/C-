#include "pullet16interpreter.h"

/***************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456
 * Class 'Interpreter' for interpreting the Pullet16 code.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Used with permission and modified by: Prashant Thirumal
 * Date: 12 November 2018
 *
 * This class does the interpretation of ASCII versions of 'executable'
 * files for the Pullet16.
 *
 * The 'Read' function reads an executable into memory, and then the
 * 'Interpret' function simulates execution of the Pullet16 using the
 * code loaded into the memory.
 *
 * Documentation for details of the functions is done as headers for
 * the functions.
 *
 * Notes:
 *   Notice throughout that 'GetTargetLocation' does the error checking
 *   for whether a target address is in fact out of bounds and also does
 *   the indirect lookup, so the code to interpret an instruction never
 *   needs to know whether the address is indirect or not.
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

/***************************************************************************
 * Function 'DoADD'.
 * This top level function interprets the 'ADD' opcode.
 *
 * Load the contents from the 'target'.
 * Convert the 16-bit value to its 2s complement version as a 32-bit value.
 * Add, storing the result in the accumulator.
 * Arithmetic overflow causes the top bits to be lost but is not flagged
 *   as an error.  It's just the way hardware works.
**/
void Interpreter::DoADD(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoADD" << endl;
#endif

  Utils::log_stream << "EXECUTE:    OPCODE ADDR TARGET " << "ADD        "
                    << addr << " " << target << endl;

  int location = this->GetTargetLocation("ADD FROM", addr, target);
//  int valuetoadd = globals_.BitStringToDec(memory_.at(location).
  int valuetoadd = DABnamespace::BitStringToDec(memory_.at(location).
                                                       GetBitPattern());
  int twoscomplement = this->TwosComplementInteger(valuetoadd);
  Utils::log_stream << "ADD VALUE " << memory_.at(location).GetBitPattern()
                    << " " << twoscomplement << endl;
  Utils::log_stream << endl;

  accum_ = (accum_ + valuetoadd) % 65536;

#ifdef EBUG
  Utils::log_stream << "leave DoADD" << endl;
#endif
}

/***************************************************************************
 * Function 'DoAND'.
 * This top level function interprets the 'AND' opcode.
 *
 * Load the contents from the 'target', taking indirection into account.
 * AND, storing the result in the accumulator.
**/
void Interpreter::DoAND(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoAND" << endl;
#endif
  Utils::log_stream << "EXECUTE:    OPCODE ADDR TARGET " << "AND "
                    << addr << " " << target << endl;

  int location = this->GetTargetLocation("AND WITH", addr, target);
//  int valuetoand = globals_.BitStringToDec(memory_.at(location).
  int valuetoand = DABnamespace::BitStringToDec(memory_.at(location).
                                                           GetBitPattern());
  Utils::log_stream << "AND VALUE " << memory_.at(location).GetBitPattern()
                    << endl;
  Utils::log_stream << endl;

  accum_ = accum_ & valuetoand;

#ifdef EBUG
  Utils::log_stream << "leave DoAND" << endl;
#endif
}

/***************************************************************************
 * Function 'DoBAN'.
 * This top level function interprets the 'BAN' opcode.
 *
 * If the accumulator value is negative, branch to the target location.
 * Otherwise, just continue on continuing on.
**/
void Interpreter::DoBAN(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoBAN" << endl;
#endif
  Utils::log_stream << "OPCODE ADDR TARGET " << "BAN " << addr << " "
                    << target << endl;

  // We are faking the twos-complement, so the 16 bit twos-complement
  // accumulator is negative if the high bit is set, which means as an
  // ordinary integer it will be greater than 32768 = 2^{15}.
  if (accum_ > 32768) {
    int location = this->GetTargetLocation("BRANCH TO", addr, target);
    pc_ = location - 1;  // a hack because we always increment later
  }

#ifdef EBUG
  Utils::log_stream << "leave DoBAN" << endl;
#endif
}

/***************************************************************************
 * Function 'DoBR'.
 * This top level function interprets the 'BR' opcode.
 *
 * Branch unconditionally to the target location.
**/
void Interpreter::DoBR(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoBR" << endl;
#endif
  Utils::log_stream << "OPCODE ADDR TARGET " << "BR  " << addr << " "
                    << target << endl;

  int location = this->GetTargetLocation("BRANCH TO", addr, target);
  pc_ = location - 1;  // a hack because we always increment later

#ifdef EBUG
  Utils::log_stream << "leave DoBR" << endl;
#endif
}

/***************************************************************************
 * Function 'DoLD'.
 * This top level function interprets the 'LD' opcode.
 *
 * Load the accumulator with the contents of the target location.
**/
void Interpreter::DoLD(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoLD" << endl;
#endif
  Utils::log_stream << "EXECUTE:    OPCODE ADDR TARGET " << "LD         "
                    << addr << " " << target << endl;

  int location = this->GetTargetLocation("LOAD FROM", addr, target);
  OneMemoryWord this_word = memory_.at(location);
//  int loadvalue = globals_.BitStringToDec(this_word.GetBitPattern());
  int loadvalue = DABnamespace::BitStringToDec(this_word.GetBitPattern());
  int twoscomplement = this->TwosComplementInteger(loadvalue);
  Utils::log_stream << "LOAD VALUE " << twoscomplement << endl;
  Utils::log_stream << endl;

  accum_ = loadvalue;

#ifdef EBUG
  Utils::log_stream << "leave DoLD" << endl;
#endif
}

/***************************************************************************
 * Function 'DoRD'.
 * This top level function interprets the 'RD' opcode.
 *
 * If there is more data:
 *   scan the next line for input
 *   convert from the hex character format input into an int value
 *   store the int value in the accumulator
 * Else:
 *   crash on read past end of file
**/
void Interpreter::DoRD(Scanner& data_scanner) {
#ifdef EBUG
  Utils::log_stream << "enter DoRD" << endl;
#endif
  Utils::log_stream << "OPCODE " << "RD  " << endl;

  if (data_scanner.HasNext()) {
    string inputstring = data_scanner.Next();
//    Hex hex = Hex(inputstring, globals_);
    Hex hex = Hex(inputstring);

    if (hex.HasAnError()) {
      Utils::log_stream << "\nERROR -- INVALID INPUT " << hex.ToString()
                        << endl;
      Utils::log_stream << "PROGRAM TERMINATING" << endl;
      exit(0);
    } else {
      accum_ = hex.GetValue();
    }
  } else {
    Utils::log_stream << "\nERROR -- READ PAST END OF FILE" << endl;
    Utils::log_stream << "PROGRAM TERMINATING" << endl;
    exit(0);
  }

#ifdef EBUG
  Utils::log_stream << "leave DoRD" << endl;
#endif
}

/***************************************************************************
 * Function 'DoSTC'.
 * This top level function interprets the 'STC' opcode.
 *
 * Get the target location.
 * Store the accumulator at that location.
 * Zero the accumulator.
 *
 * This assumes that 'GetTargetLocation' does the error checking for invalid
 * addresses.
**/
void Interpreter::DoSTC(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoSTC" << endl;
#endif
  Utils::log_stream << "EXECUTE:    OPCODE ADDR TARGET " << "STC        "
                    << addr << " " << target << endl;

  int location = this->GetTargetLocation("STORE TO", addr, target);
//  string storevalue = globals_.DecToBitString(accum_, 16);
  string storevalue = DABnamespace::DecToBitString(accum_, 16);
  // We can use 'at' here because the 'GetTargetLocation' will have
  // crashed if 'location' isn't a valid address.
  memory_.at(location).SetBitPattern(storevalue);
  Utils::log_stream << "STORE VALUE " << storevalue << endl;
  Utils::log_stream << endl;

  accum_ = 0;

#ifdef EBUG
  Utils::log_stream << "leave DoSTC" << endl;
#endif
}

/***************************************************************************
 * Function 'DoSTP'.
 * This top level function interprets the 'STP' opcode.
 *
 * Since the function one higher up interprets the 'kPCForStop' value as
 * the indicator to stop execution, all we need to do here is assign that
 * constant to the program counter.
**/
void Interpreter::DoSTP() {
#ifdef EBUG
  Utils::log_stream << "enter DoSTP" << endl;
#endif
  Utils::log_stream << "OPCODE " << "STP " << endl;

  pc_ = kPCForStop;

#ifdef EBUG
  Utils::log_stream << "leave DoSTP" << endl;
#endif
}

/***************************************************************************
 * Function 'DoSUB'.
 * This top level function interprets the 'SUB' opcode.
**/
void Interpreter::DoSUB(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoSUB" << endl;
#endif
  Utils::log_stream << "EXECUTE:    OPCODE ADDR TARGET " << "SUB        "
                    << addr << " " << target << endl;

  int location = this->GetTargetLocation("SUB FROM", addr, target);
//  int valuetosub = globals_.BitStringToDec(memory_.at(location).
  int valuetosub = DABnamespace::BitStringToDec(memory_.at(location).
                                                      GetBitPattern());
  int twoscomplement = this->TwosComplementInteger(valuetosub);
  Utils::log_stream << "SUB VALUE " << memory_.at(location).GetBitPattern()
                    << " " << twoscomplement << endl;
  Utils::log_stream << endl;

  accum_ = (accum_ - valuetosub + 65536) % 65536;

#ifdef EBUG
  Utils::log_stream << "leave DoSUB" << endl;
#endif
}

/***************************************************************************
 * Function 'DoWRT'.
 * This top level function interprets the 'WRT' opcode.
 *
 * Convert the 16-bit accumulator to a 32-bit 2s complement value.
 * Write that value to standard output.
 *
 * Note that we actually write more than just the value itself so we can do
 * better tracing. This could/should be fixed in a final version of this code.
**/
void Interpreter::DoWRT(ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter DoWRT" << endl;
#endif
  Utils::log_stream << "EXECUTE:    OPCODE             " << "WRT" << endl;

  string sss = "WRITE OUTPUT ";
  int twoscomplement = this->TwosComplementInteger(accum_);
  sss += Utils::Format(twoscomplement, 8) + " " +
                       DABnamespace::DecToBitString(accum_, 16);
//                       globals_.DecToBitString(accum_, 16);

  Utils::log_stream << sss << endl;
  out_stream << sss << endl;

#ifdef EBUG
  Utils::log_stream << "leave DoWRT" << endl;
#endif
}

/***************************************************************************
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

  Utils::log_stream << endl << endl;
  for (int i = 0; i < pc_; ++i) {
    OneMemoryWord this_word = memory_.at(i);
    string sss = "DUMP " + Utils::Format(i, 3) + " " + this_word.ToString();
    Utils::log_stream << sss << endl;
  }
  Utils::log_stream << endl << endl;

#ifdef EBUG
  Utils::log_stream << "leave DumpProgram" << endl;
#endif
}

/***************************************************************************
 * Function 'Execute'.
 * This top level function executes the code.
 *
 * Execution is basically a switch statement based on the opcode value.
 *
 * Parameters:
 *   this_word - the instance of 'OneMemoryWord' to be executed.
 *   data_scanner - the 'Scanner', needed for the 'RD' instruction
 *   out_stream - the output stream , needed for the 'WRT' instruction
**/
void Interpreter::Execute(OneMemoryWord this_word, Scanner& data_scanner,
                          ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter Execute" << endl;
#endif
  string opcode_bits = this_word.GetMnemonicBits();
  string indirect_flag = this_word.GetIndirectFlag();
  string target = this_word.GetAddressBits();

  if (opcode_bits == "000") {
    this->DoBAN(indirect_flag, target);
  } else if (opcode_bits == "001") {
    this->DoSUB(indirect_flag, target);
  } else if (opcode_bits == "010") {
    this->DoSTC(indirect_flag, target);
  } else if (opcode_bits == "011") {
    this->DoAND(indirect_flag, target);
  } else if (opcode_bits == "100") {
    this->DoADD(indirect_flag, target);
  } else if (opcode_bits == "101") {
    this->DoLD(indirect_flag, target);
  } else if (opcode_bits == "110") {
    this->DoBR(indirect_flag, target);
  } else if (opcode_bits == "111") {
    if (target == "000000000001") {
      this->DoRD(data_scanner);
    } else if (target == "000000000010") {
      this->DoSTP();
    } else if (target == "000000000011") {
      this->DoWRT(out_stream);
    } else {
      Utils::log_stream << "***** ERROR -- ILLEGAL OPCODE " << opcode_bits
                        << " AND TARGET " << this_word.ToString() << endl;
      Utils::log_stream << "PROGRAM TERMINATING" << endl;
      exit(0);
    }
  } else {
    Utils::log_stream << "***** ERROR -- ILLEGAL OPCODE " << opcode_bits
                      << " AND TARGET " << this_word.ToString() << endl;
    Utils::log_stream << "PROGRAM TERMINATING" << endl;
    exit(0);
  }

  Utils::log_stream << this->ToString() << endl;
  Utils::log_stream << endl;

#ifdef EBUG
  Utils::log_stream << "leave Execute" << endl << endl;
#endif
}

/***************************************************************************
 * Function 'FlagAddressOutOfBounds'.
 * Check to see if an address is between 0 and 'kMaxMemory' inclusive and
 * die with an error if this isn't the case.
 *
 * Parameter:
 *   address - the address to check for out of bounds
**/
void Interpreter::FlagAddressOutOfBounds(int address) {
#ifdef EBUG
  Utils::log_stream << "enter FlagAddressOutOfBounds" << endl;
#endif

//  if ((address < 0) || (address > globals_.kMaxMemory)) {
  if ((address < 0) || (address > DABnamespace::kMaxMemory)) {
    string s = "";
    s += "***** ERROR -- ADDRESS ";
    s += Utils::Format(address, 8);
    s += " IS OUT OF BOUNDS";
    Utils::log_stream << s << endl;
    exit(0);
  }

#ifdef EBUG
  Utils::log_stream << "leave FlagAddressOutOfBounds" << endl;
#endif
}

/***************************************************************************
 * Function 'GetTargetLocation'.
 * Get the target location, perhaps through indirect addressing.
 *
 * Note that this function crashes the program if the target location is out
 * of bounds for this simulated computer.
 *
 * Parameter:
 *   label - the label for our tracing output
 *   address - is this indirect or not?
 *   target - the target to look up
**/
int Interpreter::GetTargetLocation(string label, string address,
                                   string target) {
#ifdef EBUG
  Utils::log_stream << "enter GetTargetLocation" << endl;
#endif

  int location = 0;
  if (address == "0") {
//    location = globals_.BitStringToDec(target);
    this->FlagAddressOutOfBounds(location);
    Utils::log_stream << endl;
    Utils::log_stream << label << " LOCATION " << location << endl;
    Utils::log_stream << endl;
  } else {
//    location = globals_.BitStringToDec(target);
    location = DABnamespace::BitStringToDec(target);
    this->FlagAddressOutOfBounds(location);
//    int indirectlocation = globals_.BitStringToDec(memory_.at(location).
    int indirectlocation = DABnamespace::BitStringToDec(memory_.at(location).
                                                  GetAddressBits());
    this->FlagAddressOutOfBounds(indirectlocation);
    Utils::log_stream << endl;
    Utils::log_stream << label << " LOCATION " << location << endl;
    Utils::log_stream << label << " INDIRECT " << indirectlocation << endl;
    Utils::log_stream << endl;
    location = indirectlocation;
  }

#ifdef EBUG
  Utils::log_stream << "leave GetTargetLocation" << endl;
#endif

  return location;
}

/***************************************************************************
 * Function 'HW5Binary'.
 * This top level function
 *   1. Reads the ASCII of the executable into a 'vector'
 *   2. Dumps the ASCII from the 'vector'
 *   3. Converts the ASCII to binary and stores in a 'vector'
 *   4. Dumps the binary from the 'vector'
 *   5. Writes the binary to a file
 *   6. Reads back the binary from the file and expands the binary
 *      into a new vector of ASCII strings
 *   7. Checks that the new vector is identical to the old vector
 *
 * We pass in the filename instead of an open file because there are
 * multiple ways to open and read the file and we do not want to be
 * constrained in our choices.
 *
 * Parameters:
 *   adotout_filename - the name of the file to read
**/
void Interpreter::HW5Binary(string adotout_filename) {
#ifdef EBUG
  Utils::log_stream << "enter HW5Binary" << endl;
#endif


  // Initialize the variables and the scanner

  vector<string> ascii_strings;
  vector<string> new_ascii;
  vector<int16_t> binary_;
  string data;
  string output = "binary_out.txt";
  bool comparison;
  Scanner data_scanner;

  // Open the file
  // Read each line from the file and push it into the vector

  Utils::log_stream << "Opening ASCII file\n" << endl;
  data_scanner.OpenFile(adotout_filename + ".txt");

  Utils::log_stream << "Reading ASCII\n " << endl;
  while (data_scanner.HasNext()) {
    data = data_scanner.NextLine();
    ascii_strings.push_back(data);
  }

  data_scanner.Close();

  // Dump the ASCII

  Utils::log_stream << "Dumping ASCII\n" << endl;
  for (unsigned int i = 0; i < ascii_strings.size(); ++i) {
    Utils::log_stream << ascii_strings.at(i) << endl;
  }

  // Convert the ascii strings to binary
  // Push the converted binary into the vector
  // Dump the binary from the vector

  Utils::log_stream << "Converting ASCII to Binary\n" << endl;
  for (unsigned int i = 0; i < ascii_strings.size(); i++) {
    int temp = DABnamespace::BitStringToDec(ascii_strings.at(i));
    binary_.push_back(temp);
  }

  Utils::log_stream << "Dumping the Binary\n" << endl;
  for (unsigned int i = 0; i < binary_.size(); ++i) {
    Utils::log_stream << binary_.at(i) << endl;
  }

  // Write the binary to the output file

  ofstream file(output);
  Utils::log_stream << "Writing the binary to a file \n" << endl;
  for (unsigned int i = 0; i < binary_.size(); ++i) {
    file << binary_.at(i) << endl;
  }
  file.close();

  // Read back the binary from the file
  // Expand the binary to a new vector of ASCII strings

  Utils::log_stream << "Reading the binary from the output file\n" << endl;
  data_scanner.OpenFile(output);

  Utils::log_stream << "Expanding binary to vector of ASCII strings\n" << endl;
  while (data_scanner.HasNext()) {
    int binary_data = data_scanner.NextInt();
    string converted_ = DABnamespace::DecToBitString(binary_data, 16);
    new_ascii.push_back(converted_);
  }

  data_scanner.Close();

  // Outputting it to a bin file

  std::ifstream in(output);
  std::ofstream out("binary_output.bin", std::ios::binary);

  int bin_numbers;
  while (in >> bin_numbers) {
    out.write(reinterpret_cast<char*> (&bin_numbers), sizeof bin_numbers);
  }

  // Compare the two vectors of ascii strings

  Utils::log_stream << "Comparing the two vectors of ASCII strings\n" << endl;
  for (unsigned int i = 0; i < ascii_strings.size(); ++i) {
    if ( ascii_strings.at(i) == new_ascii.at(i) ) {
      comparison = true;
      Utils::log_stream << "GOOD: " << endl;
      Utils::log_stream << "Original: " + ascii_strings.at(i)
      + "Re-Written: " + new_ascii.at(i) << endl;
    } else { comparison = false;
             Utils::log_stream << "BAD: " << endl;
             Utils::log_stream << "Original: " + ascii_strings.at(i)
             +  "   " + "Re-Written: " + new_ascii.at(i) << endl;
           }
  }



#ifdef EBUG
  Utils::log_stream << "leave HW5Binary" << endl;
#endif
}

/***************************************************************************
 * Function 'Interpret'.
 * This top level function interprets the code.
 *
 * Note that errors in execution will result in the program being terminated
 * from functions other than this one.
 *
 * We run a loop until we either hit the bogus PC value for the STP or we
 * encounter an error, which can include having the PC go past 4095.
 * So the interpreter runs a logically infinite loop:
 *
 * while true (execution should stop with a 'STP' instruction)
 *   decode the instruction pointed to by the PC
 *   execute the instruction
 *   check for invalid PC or infinite loop
**/
void Interpreter::Interpret(Scanner& data_scanner, ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter Interpret" << endl;
#endif
  // Loop through the memory.
  // Unpack the instruction.
  // Execute the instruction.
  //
  // The only gotcha in this program is that we ALWAYS bump the PC by 1 at
  // the bottom of this loop.  For the branching instructions, we thus have
  // to put a value one smaller in the PC, and let the bottom of this loop
  // then increment it to be the correct PC value.
  int instructioncount = 0;
  pc_ = 0;
  while (true) {  // run forever, break below for STP or instructioncount
    OneMemoryWord this_word = memory_.at(pc_);
    Utils::log_stream << "INTERPRET: PC OPCODE ADDR TARGET "
                      << Utils::Format(pc_, 6) << " "
                      << this_word.ToString() << endl;
    this->Execute(this_word, data_scanner, out_stream);

    // If we have hit the stop we will have returned a flag value that says
    // we should stop execution. Note that the 'kPCForStop' value is one
    // larger than the largest 16-bit value, so it can be detected on an
    // interpreter running on a 32-bit or 64-bit processor and can't possibly
    // be used as "data" for some other instruction.
    if (pc_ == kPCForStop) {
      break;
    }

    ++pc_;
    // If we have executed but the PC is now incremented past the end of
    // memory, we have an execution error.
    if (pc_ >= memory_.size()) {
      Utils::log_stream << "***** ERROR -- PC BEYOND MEMORY BOUND" << endl;
      break;
    }

    // This is an interpreter thing. We prevent infinite loops from being
    // interpreted by having a timeout feature on instruction count.
    ++instructioncount;
    if (instructioncount >= kMaxInstrCount) {
      Utils::log_stream << "PROGRAM TIMED OUT" << endl;
      break;
    }
  }  // while (true)

#ifdef EBUG
  Utils::log_stream << "leave Interpret" << endl;
#endif
}

/***************************************************************************
 * Function 'ReadProgram'.
 * This top level function reads the ASCII of the machine code, one line
 * at a time, uses the input to create an instance of 'OneMemoryWord', and
 * stores that instance into memory.
 *
 * Parameters:
 *   in_scanner - the scanner to read for source code
**/
void Interpreter::ReadProgram(Scanner& in_scanner) {
#ifdef EBUG
  Utils::log_stream << "enter ReadProgram" << endl;
#endif

  accum_ = 0;
  pc_ = 0;

  // Read the lines of the ASCII version of the executable and put the
  // ASCII into a 'vector' of 'OneMemoryWord' instances.
  int linesub = 0;
  while (in_scanner.HasNext()) {
    string line = in_scanner.NextLine();
//    OneMemoryWord one_word = OneMemoryWord(globals_, line);
    OneMemoryWord one_word = OneMemoryWord(line);
    memory_.push_back(one_word);
    ++linesub;
    ++pc_;
    Utils::log_stream << "READ " << linesub << " " << pc_ << " "
                      << line << endl;
  }  // while (in_scanner.HasNext()) {

  Utils::log_stream << this->ToString() << endl;

#ifdef EBUG
  Utils::log_stream << "leave ReadProgram" << endl;
#endif
}

/***************************************************************************
 * Function 'ToString'.
 *
 * This outputs a prettyprinted string:
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

  string stars40 = "********* ********* ********* ********* ";
  string sss = "\n" + stars40 + stars40 + "\n";
  sss += "MACHINE IS NOW\n";

  sss += "PC    " + Utils::Format(pc_, 8) + "\n";

  int twoscomplement = this->TwosComplementInteger(accum_);
  sss += "ACCUM " + Utils::Format(twoscomplement, 8)
//                  + " " + globals_.DecToBitString(accum_, 16)
                  + " " + DABnamespace::DecToBitString(accum_, 16)
                  + "\n\n";

  int memorysize = memory_.size();
  for (int outersub = 0; outersub < memorysize; outersub += 4) {
    sss += "MEM " + Utils::Format(outersub, 4)
                + "-"
                + Utils::Format(outersub+3, 4);
    for (int innersub = outersub; innersub < outersub + 4; ++innersub) {
      if (innersub < memorysize) {
        sss += " " + memory_.at(innersub).GetBitPattern();
      }
    }
    sss += "\n";
  }
  sss += "\n" + stars40 + stars40;

#ifdef EBUG
  Utils::log_stream << "leave ToString" << endl;
#endif

  return sss;
}

/***************************************************************************
 * Function 'TwosComplementInteger'.
 *
 * This converts a 16 bit integer into the plus or minus 15 bit integer
 * that is the integer of the 2s complement bit pattern.
 *
 * Parameter:
 *   what - the integer value to convert to the integer that is the 2s compl
 *
 * Returns:
 *   the converted value
**/
int Interpreter::TwosComplementInteger(int what) {
#ifdef EBUG
  Utils::log_stream << "enter TwosComplementInteger" << endl;
#endif

  int twoscomplement = (what > 32768) ? what - 65536 : what;

#ifdef EBUG
  Utils::log_stream << "leave TwosComplementInteger" << endl;
#endif

  return twoscomplement;
}

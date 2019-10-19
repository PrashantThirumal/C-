/****************************************************************
 * Header file for the Pullet16 interpreter.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Ued with permission and modified by: Prashant Thirumal
 * Date: 21 October 2018
**/

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::string;
using std::vector;

#include "../../Utilities/scanner.h"
#include "../../Utilities/scanline.h"
#include "../../Utilities/utils.h"

#include "dabnamespace.h"
#include "onememoryword.h"
#include "hex.h"

class Interpreter {
 public:
  Interpreter();
  virtual ~Interpreter();

  void DumpProgram(ofstream& out_stream);
  void Interpret(Scanner& data_scanner, ofstream& out_stream);
  void ReadProgram(Scanner& infile_scanner);

 private:
  static const int kMaxInstrCount = 128;
  static const int kPCForStop = 65537;  // 16-bit overflow value

  int pc_;
  int accum_;

  string ToString();

  vector<OneMemoryWord> memory_;

  map<string, string> code_to_mnemonic_ = { {"000", "BAN"},
                                            {"001", "SUB"},
                                            {"010", "STC"},
                                            {"011", "AND"},
                                            {"100", "ADD"},
                                            {"101", "LD "},
                                            {"110", "BR "},
                                            {"111", "EEE"}  // STP, RD, WRT
                                          };
  map<string, string> mnemonic_to_code_ = { {"BAN", "000"},
                                            {"SUB", "001"},
                                            {"STC", "010"},
                                            {"AND", "011"},
                                            {"ADD", "100"},
                                            {"LD ", "101"},
                                            {"BR ", "110"},
                                            {"STP", "111"},
                                            {"RD ", "111"},
                                            {"WRT", "111"}
                                          };

  string Decode(string the_ascii);
  void DoADD(string addr, string target);
  void DoAND(string addr, string target);
  void DoBAN(string addr, string target);
  void DoBR(string addr, string target);
  void DoLD(string addr, string target);
  void DoRD(Scanner& data_scanner);
  void DoSTC(string addr, string target);
  void DoSTP();
  void DoSUB(string addr, string target);
  void DoWRT(ofstream& out_stream);
  void Execute(OneMemoryWord this_word,
               Scanner& data_scanner, ofstream& out_stream);
  void FlagAddressOutOfBounds(int address);
  int GetTargetLocation(string label, string address, string target);
  int TwosComplementInteger(int value);
};
#endif

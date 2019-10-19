#include "main.h"

/****************************************************************
 * Main program for Homework 5.  Read and write the binary
 * that matches the ASCII version of an 'a.out' file.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Used with permission and modified by: Jane Random Hacker
 * Date: 17 August 2018
 *
 * Processing is as follows:  We take in the 'a.out' file name,
 *     the data file name, the output file name, and a log file
 *     name as command line arguments.  We open the files, read
 *     and dump the 'a.out' file to the log, and then run an
 *     interpreter for the Pullet16 on the 'a.out' executable.
 *
 * CAVEAT:  Note that the data file name is a REQUIRED command
 *          line argument, so some (dummy?) file must be supplied.
 *
 * CAVEAT:  Note that the 'a.out' file name does not have an
 *          extension.  This is to allow the same source code
 *          to run without modification by adding the 'BINARY'
 *          for the ifdef at compile time.
**/

static const char kTag[] = "MAIN: ";

int main(int argc, char *argv[]) {
  string adotout_filename = "dummyadotoutfilename";
  string data_filename = "dummydatafilename";
  string out_filename = "dummyoutname";
  string log_filename = "dummylogname";

  Scanner adotout_scanner;
  Scanner data_scanner;
  ofstream out_stream;

  Interpreter interpreter;

  Utils::CheckArgs(4, argc, argv,
                   "adotoutfilename datafilename outfilename logfilename");
  adotout_filename = static_cast<string>(argv[1]);
  data_filename = static_cast<string>(argv[2]);
  out_filename = static_cast<string>(argv[3]);
  log_filename = static_cast<string>(argv[4]);

  Utils::LogFileOpen(log_filename);
//  adotout_scanner.OpenFile(adotout_filename);
  data_scanner.OpenFile(data_filename);
  Utils::FileOpen(out_stream, out_filename);

  Utils::log_stream << kTag << "Beginning execution" << endl;
  Utils::log_stream << kTag << "adotoutfile   '" << adotout_filename
                            << "'" << endl;
  Utils::log_stream << kTag << "datafile '" << data_filename << "'" << endl;
  Utils::log_stream << kTag << "outfile  '" << out_filename << "'" << endl;

  interpreter.HW5Binary(adotout_filename);
//  interpreter.ExpandBinaryToASCII();
//  interpreter.ReadProgram(adotout_scanner);
//  adotout_scanner.Close();
//  interpreter.DumpProgram(out_stream);
//  interpreter.Interpret(data_scanner, out_stream);

  Utils::log_stream << kTag << "Ending execution" << endl;

  Utils::FileClose(out_stream);
  Utils::FileClose(Utils::log_stream);

  return 0;
}

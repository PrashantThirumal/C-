#include "main.h"
/****************************************************************
 * Main program for Homework 3.  Read an file that is an ASCII
 * version of a program for the Pullet16.
 *
 * Author/copyright:  Duncan Buell. All rights reserved.
 * Used with permission and modified by: Prashant Thirumal
 * Date: 2 October 2018
**/

// static const string kTag = "Main: ";
static const char kTag[] = "Main: ";

int main(int argc, char *argv[]) {
  string in_filename = "dummyinfilename";
  string out_filename = "dummyoutname";
  string log_filename = "dummylogname";

  Scanner in_scanner;
  ofstream out_stream;

  Interpreter interpreter;

  // Check the arguments and convert to strings.
  Utils::CheckArgs(3, argc, argv, "infilename outfilename logfilename");
  in_filename = static_cast<string>(argv[1]);
  out_filename = static_cast<string>(argv[2]);
  log_filename = static_cast<string>(argv[3]);

  // Open the log file and the scanner file.
  Utils::LogFileOpen(log_filename);
  in_scanner.OpenFile(in_filename);
  Utils::FileOpen(out_stream, out_filename);

  Utils::log_stream << kTag << "Beginning execution" << endl;
  Utils::log_stream << kTag << "infile  '" << in_filename << "'" << endl;
  Utils::log_stream << kTag << "outfile '" << out_filename << "'" << endl;

  // Read, dump, close.
  interpreter.ReadProgram(in_scanner);
  interpreter.DumpProgram(out_stream);
  in_scanner.Close();

  // Close up and go home.
  Utils::log_stream << kTag << "Ending execution" << endl;

  Utils::FileClose(out_stream);
  Utils::FileClose(Utils::log_stream);

  return 0;
}

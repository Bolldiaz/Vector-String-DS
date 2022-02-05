/**
 *      C/CPP 2021 Summer - Ex06       *
 * Bonus part tests - vl_string - v1.2 *
 *           By Ohad Klein             *
 */

#include <iostream>
#include "vl_string.h"
#include <cassert>
#include <cstring>

void test_def_ctor ()
{
  vl_string<> vl_str;
  assert (vl_str.capacity () == 16);
  assert (vl_str.size () == 0);
  assert(vl_str.empty ());
  assert(vl_str.at(0) == '\0');

  const vl_string<5> vl_str_const;
  assert (vl_str_const.capacity () == 5);
  assert (vl_str_const.size () == 0);
  assert (vl_str_const.empty ());
  assert(vl_str_const.at(0) == '\0');
}

void test_implicit_and_copy_ctor ()
{
  // implicit:
  vl_string<> vl_str_imp = "Hello, World!";
  assert (vl_str_imp.capacity () == 16);
  assert (vl_str_imp.size () == 13);
  assert(!vl_str_imp.empty ());
  assert(vl_str_imp.at(13) == '\0');

  const vl_string<5> vl_str_imp_const = "Hello";
  assert (vl_str_imp_const.capacity () == (int)((5+1) * 3 / 2 ));
  assert (vl_str_imp_const.size () == 5);
  assert(!vl_str_imp_const.empty ());
  assert(vl_str_imp_const.at(5) == '\0');

  // copy:
  vl_string<> vl_str_copy = vl_str_imp;
  assert (vl_str_copy.capacity () == vl_str_imp.capacity ());
  assert (vl_str_copy.size () == 13);
  assert(!vl_str_copy.empty ());

  const vl_string<5> vl_str_copy_const = vl_str_imp_const;
  assert (vl_str_copy_const.capacity () == vl_str_imp_const.capacity ());
  assert (vl_str_copy_const.size () == 5);
  assert(!vl_str_copy_const.empty ());
}

void test_vl_vector_methods ()
{
  vl_string<> vl_str = "Never gonna";
  assert (vl_str.capacity () == 16);
  assert (vl_str.size () == 11);
  assert(vl_str.at(11) == '\0');

  vl_str.push_back (' ');
  assert (vl_str.capacity () == 16);
  assert (vl_str.size () == 12);

  // note that end() must point to after the '\0' character:
  const char *str = "give you up\n";
  vl_str.insert (vl_str.end () - 1, str, str + strlen (str));
  assert(vl_str.capacity () == (int) ( 25 * 3 / 2 ));
  assert (vl_str.size () == 24);
  assert(vl_str.at(24) == '\0');
  assert(vl_str.contains ("Never gonna give you up\n"));

  vl_str.pop_back ();
  assert(vl_str.capacity () == (int)(3 * 25 / 2));
  assert (vl_str.size () == 23);
  assert(!vl_str.contains ("\n"));
  assert(vl_str.at (23) == '\0');

  // the following test also checks if '\0' is at the end of the container
  vl_str.erase (vl_str.begin (), vl_str.begin () + 7);
  assert(vl_str.capacity () == (int)(3 * 25 / 2));
  assert (vl_str.size () == 16);
  vl_str.erase (vl_str.begin ());
  assert(vl_str.capacity () == 16);
  assert (vl_str.size () == 15);
  assert(!vl_str.contains ("Never"));

  vl_str.clear ();
  assert(vl_str.capacity () == 16);
  assert (vl_str.size () == 0);
  assert(vl_str.empty ());
  assert(vl_str.contains ("\0"));
}

void test_contains_method ()
{
  vl_string<> vl_str;
  assert(vl_str.contains (""));
  vl_str = "Never gonna let you down\n";
  assert(vl_str.contains ("Never "));
  assert(!vl_str.contains (" Never"));
  assert(vl_str.contains ("down\n"));
  assert(!vl_str.contains ("nwod"));
  assert(vl_str.contains ("a"));
  assert(!vl_str.contains ("b"));
  assert(vl_str.contains (""));
  assert(vl_str.contains ("\0"));
  assert(vl_str.contains ("Never gonna let you down\n"));
  vl_str.clear();
  assert(vl_str.contains (""));
  assert(vl_str.contains ("\0"));
}

void test_vl_vector_operators ()
{
  vl_string<> vl_str = "Never gonna run around and desert you\n";
  // indexing operators:
  try
    {
      assert(vl_str.at (0) == 'N');
      assert(vl_str.at (37) == '\n');
    }
  catch (std::out_of_range &e)
    {
      assert(e.what () && false);
    }
  // at 38 ('\0'), at should be able to access
  bool exception_caught = false;
  try
    {
      vl_str.at (38);
    }
  catch (std::exception &e)
    {
      exception_caught = true;
    }
  assert(!exception_caught && "method .at threw exception at '\\0'");

  // at 39(out of range), at should not be able to access
  exception_caught = false;
  try
    {
      vl_str.at (39);
    }
  catch (std::exception &e)
    {
      exception_caught = true;
    }
  assert(exception_caught && "method .at didn't throw exception");
  assert(vl_str[0] == 'N');
  assert(vl_str[37] == '\n');
  vl_str[37] = '!';
  assert(vl_str[37] == '!');
  try
    {
      vl_str.at (0) = 'G';
    }
  catch (std::exception &e)
    {
      assert(e.what () && false);
    }
  assert(vl_str.at(0) == 'G');

  // copy assignment operator:
  vl_string<> vl_str_to_copy = "Never gonna make you cry\n";
  vl_str = vl_str_to_copy;
  assert(vl_str.capacity() == vl_str_to_copy.capacity());
  assert(vl_str.size() == vl_str_to_copy.size());
  assert(vl_str.contains ("Never gonna make you cry\n"));
  assert(!vl_str.contains ("Never gonna run around and desert you\n"));
  vl_str[0] = 'G';
  assert(vl_str.contains ("Gever"));
  assert(!vl_str_to_copy.contains ("Gever"));
  assert(vl_str != vl_str_to_copy);
  vl_str[0] = 'N';
  assert(vl_str == vl_str_to_copy);
}

void test_addition_operators ()
{
  vl_string<> vl_str;
  vl_string<> vl_str_to_add = "Never gonna";
  (((vl_str += vl_str_to_add) += ' ') += "say goodbye") += '\n';
  assert(vl_str.contains ("Never gonna say goodbye\n"));

  vl_string<> vl_str_2;
  vl_str_2 = vl_str_to_add + ' ' + "tell a lie and hurt you" + '\n';
  assert(vl_str_2.contains ("Never gonna tell a lie and hurt you\n"));
}

void test_cast ()
{
  vl_string<> vl_str = "Fifteen chars..";
  const char *str = vl_str;
  assert(strcmp(str, "Fifteen chars..") == 0);
  assert(strlen(str) == strlen("Fifteen chars.."));
  const vl_string<> vl_str_const = "Sixteen chars...";
  const char *str_2 = vl_str_const;
  assert(strlen(str_2) == strlen("Sixteen chars..."));
  assert(strcmp(str_2, "Sixteen chars...") == 0);
}

int main ()
{
  std::cout << "~~~~~~~~~~~~~~~~~ Testing ~~~~~~~~~~~~~~~~~" << std::endl;
  std::cout << std::endl;
  std::cout << "********* Starting test_def_ctor **********" << std::endl;
  test_def_ctor ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "*** Starting test_implicit_and_copy_ctor **" << std::endl;
  test_implicit_and_copy_ctor ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "***** Starting test_vl_vector_methods *****" << std::endl;
  test_vl_vector_methods ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "****** Starting test_contains_method ******" << std::endl;
  test_contains_method ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "**** Starting test_vl_vector_operators ****" << std::endl;
  test_vl_vector_operators ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "***** Starting test_addition_operators ****" << std::endl;
  test_addition_operators ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "*********** Starting test_cast ************" << std::endl;
  test_cast ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;
  std::cout << "~~~~~~~~~~~ No errors detected! ~~~~~~~~~~~" << std::endl;

  return 0;
}
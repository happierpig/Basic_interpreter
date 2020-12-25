/*
 * File: main.h
 * ------------
 * This file renames the <code>main</code> method in the client's
 * program to <code>Main</code>, thereby allowing a custom
 * <code>main</code> method in the libraries to take control
 * before passing control back to the client program.  The main macro
 * also defines a function getMainFlags that returns an int whose bits
 * indicate which of the various interfaces have been loaded by this
 * definition of main.
 *
 * Note: This file can be loaded more than once and must therefore
 * check to see what has already been defined.
 */

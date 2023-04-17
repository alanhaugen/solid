#!/usr/bin/env janet

(defn print-greeting
  [greetee]
  (print "Hello, " greetee "!"))

(defn main
  [& args]
  (pp args) # pretty-prints the args tuple
  (print (length args))
  (print-greeting (get args 1)))

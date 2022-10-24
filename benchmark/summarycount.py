#!/usr/bin/python3
import glob
import os

files = glob.glob('results/bm*.txt')
bm_file = max(files, key=os.path.getmtime)

summary_file = "results/summary-" + bm_file[len('results/bm')+1:]

fa = open(summary_file, "a")
fr = open(summary_file, "r")

good = 0
optimal = 0
bad = 0
error = 0

for line in fr.readlines():
    word_list = line.split(" --- ")

    if len(word_list) == 1: # Last line contains only "-"
        break

    rating = word_list[1]

    if rating == "Good\n":
        good += 1
    elif rating == "Optimal!\n":
        optimal += 1
    elif rating == "Needs improvement\n":
        bad += 1
    elif rating == "Not found\n":
        error += 1
    
fa.write("\nTOTAL COUNT\n")
fa.write("Optimal: ")
fa.write(str(optimal))
fa.write("\n")
fa.write("Good: ")
fa.write(str(good))
fa.write("\n")
fa.write("Needs improvement: ")
fa.write(str(bad))
fa.write("\n")
fa.write("Not found: ")
fa.write(str(error))
fa.write("\n")

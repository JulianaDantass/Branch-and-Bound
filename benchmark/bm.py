#!/usr/bin/python3
import sys
import glob
import os

if len(sys.argv) > 2:
    print("Unexpected arguments")
    exit()
elif len(sys.argv) < 2:
    print("Missing argument")
    exit()

# grab the latest benchmark data
list_of_files = glob.glob('results/bm*.txt') 
bm_file = max(list_of_files, key=os.path.getmtime)

results_file = "results/results-" + bm_file[len('results/bm')+1:]
summary_file = "results/summary-" + bm_file[len('results/bm')+1:]

fin = open(bm_file, "r")              # Results from running tsp 10 times for each instance
fout = open(results_file, "w+")        # Output file
ftarget = open("target_data", "r")       # Target average results
fsummary = open(summary_file, "w+")    # Results summary, looking at off percentage

isFirst = True
flag = 0 # 0 is cost 1 is time
sum_cost = 0
sum_time = 0

for line in fin.readlines():

    #Treating read line
    no_endl = line.split("\n")
    my_list = no_endl[0].split()

    if len(my_list) == 1: # Line is an instance's name

        instance_name = my_list[0]

        if isFirst == False and tgt_find: # There's no cost or time data at the first name
            avg_cost = sum_cost / int(sys.argv[1])
            avg_time = sum_time / int(sys.argv[1])

            try:
                off_pct = ( (avg_cost - target) / target ) * 100
            except TypeError:
                if avg_cost <= target[1]:
                    off_pct = 0.0
                else:
                    avg_1 = ( (avg_cost - target[0]) / target[0] ) * 100    
                    avg_2 = ( (avg_cost - target[1]) / target[1] ) * 100
                    avg_1 = str(avg_1)
                    avg_2 = str(avg_2)
                    # formating the values
                    avg_1 = avg_1[:5]
                    avg_2 = avg_2[:5]

                    off_pct = avg_1 + "% ~ " + avg_2
                    off_pct_rg = [float(avg_1), float(avg_2)]

            # Verbose - Writes info to results.txt
            fout.write("\nAvg cost: ")
            fout.write(str(avg_cost))
            fout.write("\nAvg time: ")
            fout.write(str(avg_time))
            fout.write("\nOff percentage: ")
            fout.write(str(off_pct)+"%")
            fout.write("\n\n")

            # Summary - Writes info to summary.txt
            fsummary.write(" --- ")
            if off_pct <= 0:
                fsummary.write("Optimal!")
            else:
                try:
                    if off_pct <= 0.5:
                        fsummary.write("Good")
                    else:
                        fsummary.write("Needs improvement")
                except TypeError:
                    if off_pct_rg[0] <= 0.5:
                        fsummary.write("Good")
                    else:
                        fsummary.write("Needs improvement")

            fsummary.write("\n")


        # Gets target value from target.txt for given instance
        # so the error % can be calculated later.
        for tgt in ftarget.readlines():

            #Treating read line
            tgt_list = tgt.split(":")
            tgt_name = tgt_list[0]

            tgt_find = 0

            if instance_name == tgt_name:
                tgt_find = 1

                # Continue treating the line (after if to save resources)
                tgt_value_endl = tgt_list[1]
                tgt_value_list = tgt_value_endl.split("\n")
                tgt_value = tgt_value_list[0]

                try:
                    target = float(tgt_value)
                except ValueError:
                    # Case of result as a range
                    target = tgt_value.split(',')

                    x1 = target[0].split('[')
                    x1 = x1[1]

                    x2 = target[1].split(']')
                    x2 = x2[0]

                    target = [float(x1), float(x2)]


                break

        ftarget.seek(0,0)

        if instance_name == "-":
            exit(1)

        if not tgt_find:
            message_results = "Couldn't find optimal data for " + instance_name + "\n\n"
            message_summary = instance_name + " --- Not found\n"
            fout.write(message_results)
            fsummary.write(message_summary)
        else:
            fout.write(instance_name)
            fsummary.write(instance_name)

        #Reset total cost and time
        sum_cost = 0
        sum_time = 0

    elif tgt_find: # Else line is cost or time
        num = my_list[1]

        if flag == 0:
            sum_cost += int(num)
            flag = 1
        elif flag == 1:
            sum_time += float(num)
            flag = 0

    isFirst = False


fin.close()
fout.close()
ftarget.close()
fsummary.close()

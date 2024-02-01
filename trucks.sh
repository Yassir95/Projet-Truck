#!/bin/bash
sources="./progc"
executable="$sources/main"

compiled=0

test_executable() {
    need_compile=0

    if [ $1=1 ]; then
        make -C $sources clean > /dev/null 2>&1
        need_compile=$1
    fi

    if [ -f "$executable" -a $need_compile=0 ]; then
        echo "compile: no need to recompile $executable."
    else
        echo "compile: recompiling $executable..."
        
        make -C $sources > /dev/null 2>&1
        
        if [ $? -eq 0 ]; then
            echo "compile: $executable has been compiled successfully." 
            compiled=1
        else
            >&2 echo "error: an error occured while compiling the C program."
            exit 1
        fi
    fi
}

test_folder() {
    # create the images folder if it isn't done already
    if [ ! -d "$1" ]; then
        # create the dir
        mkdir -p "$1"

        if [ $? -ne 0 ]; then
            >&2 echo "error: an error occured while creating the $1 folder."
        fi
        echo "folder: $1 has been created."
    else
        echo "folder: $1 already exists."
    fi
}

show_help() {
    echo "Usage: ./trucks.sh [options] [file]"
    echo "Options:"
    echo "  -h: show this help message"
    echo "  -c: force to re-compile the C program"
    echo "  -d1 [x]: retrieve the list of the first x (default 10) drivers with the more different routes"
    echo "  -d2 [x]: retrieves the list of the first x (default 10) drivers to cover the greatest distance"
    echo "  -l [x]: retrieves list of x (default 10) longest routes"
    echo "  -t [x]: retrieves the list of x (default 10) most-visited cities." 
    echo "  -s [start] [end]: retrieves the list of the routes where their IDs are between the start and end and resume their length, minimum and maximum step distance."
}

clean_folders() {
    echo "clean: cleaning folders..."
    rm -rf temp/*
    rm -rf data/*
    echo "clean: folders have been cleaned."
}

exit_properly() {
    clean_folders
    echo "exiting..."
    exit 1
}

last_time=0

start_timer() {
    last_time=$(date +%s)
}

end_timer() {
    current_time=$(date +%s)
    echo "timer: $(($current_time - $last_time)) seconds elapsed."
}

OPTIND=1

# if help is asked, show help and exit
while getopts ":h" opt; do   
  case "$opt" in
    h)
        show_help
        exit_properly
      ;;
  esac
done

# get the filepath in firt argument and test if it exists
if [ -z $1 ]; then
    >&2 echo "error: you must specify a file in the first argument."
    show_help
    exit_properly
fi

if [ ! -f $1 ]; then
    echo "The file $1 doesn't exist."
    exit_properly
fi

# test that the file is a CSV file
if [ ${1: -4} != ".csv" ]; then
    >&2 echo "error: the file must be a CSV file."
    exit_properly
fi

# test if the csv has the right number of columns
if [ $(head -n 1 $1 | tr ';' '\n' | wc -l) -ne 6 ]; then
    >&2 echo "error: the file must have 6 columns."
    exit_properly
fi

test_folder "images"
test_folder "temp"
test_folder "data"
test_folder "demo"

target_file=$1
copied_file="data/$(basename $target_file)"

# copy target file to data folder
echo "data: copying $target_file to $copied_file..."
cp $target_file $copied_file
echo "data: $target_file has been copied to $copied_file."

OPTIND=2

# if compile is asked, compile and continue
while getopts ":c" opt; do
  case "$opt" in
    c)
        test_executable 1
      ;;
  esac
done

shift

# test if gnuplot is installed
if ! command -v gnuplot >/dev/null 2>&1; then
    >&2 echo "error: gnuplot is not installed."
    exit_properly
fi

# checking for others options
while [ $# -gt 0 ]; do

    case $1 in
    "-d1")
        option_arg=10

        # if the option has an argument, get it
        if [ $# -gt 1 ]; then
            # if the argument is a number, get it
            if [ $2 -eq $2 ] 2> /dev/null; then
                option_arg=$2
                shift
            fi
        fi

        # check option_arg >= 1
        if [ $option_arg -lt 1 ]; then
            >&2 echo "error: the number of cities must be greater than 0."
            exit_properly
        fi

        start_timer
        echo "demo: generating the first $option_arg drivers with the more different routes..."
        awk -F";" '$2 == 1 { sum[$6] += 1 } END { for (i in sum) printf "%s;%d\n", i, sum[i] }' ${copied_file} | sort --parallel 16 -t';' -k2nr | head -$option_arg > "temp/d1_tmp.csv"
        echo "demo: the first $option_arg drivers with the more different routes have been generated." 
        end_timer
        
        gnuplot ./progc/gnuplot/d1_plot.gnu

        echo "created: images/d1_image.png"

        ;;
    "-d2")
        option_arg=10

        # if the option has an argument, get it
        if [ $# -gt 1 ]; then
            # if the argument is a number, get it
            if [ $2 -eq $2 ] 2> /dev/null; then
                option_arg=$2
                shift
            fi
        fi

        # check option_arg >= 1
        if [ $option_arg -lt 1 ]; then
            >&2 echo "error: the number of cities must be greater than 0."
            exit_properly
        fi

        start_timer
        echo "demo: generating the first $option_arg drivers to cover the greatest distance..."
        awk -F";" '{ sum[$6] += $5 } END { for (i in sum) printf "%s;%d\n", i, sum[i] }' ${copied_file} | sort --parallel 16 -t';' -k2nr | head -$option_arg > "temp/d2_tmp.csv"
        echo "demo: the first $option_arg drivers to cover the greatest distance have been generated."
        
        gnuplot ./progc/gnuplot/d2_plot.gnu

        echo "created: images/d2_image.png"

        end_timer
        ;;   
    "-l")
        option_arg=10

        # if the option has an argument, get it
        if [ $# -gt 1 ]; then
            # if the argument is a number, get it
            if [ $2 -eq $2 ] 2> /dev/null; then
                option_arg=$2
                shift
            fi
        fi

        # check option_arg >= 1
        if [ $option_arg -lt 1 ]; then
            >&2 echo "error: the number of cities must be greater than 0."
            exit_properly
        fi

        start_timer
        echo "demo: generating the first $option_arg longest routes..."
        awk -F";" '{ sum[$1] += $5 } END { for (i in sum) printf "%s;%d\n", i, sum[i] }' ${copied_file} | sort --parallel 16 -t';' -k2nr | head -$option_arg > "temp/l_tmp.csv"
        echo "demo: the first $option_arg longest routes have been generated."
        
        gnuplot ./progc/gnuplot/l_plot.gnu

        echo "created: images/l_image.png"

        end_timer
      ;;
    "-t")
        option_arg=10

        # if the option has an argument, get it
        if [ $# -gt 1 ]; then
            # if the argument is a number, get it
            if [ $2 -eq $2 ] 2> /dev/null; then
                option_arg=$2
                shift
            fi
        fi

        # check option_arg >= 1
        if [ $option_arg -lt 1 ]; then
            >&2 echo "error: the number of cities must be greater than 0."
            exit_properly
        fi

        if [ $compiled -eq 0 ]; then
            test_executable
        fi

        start_timer
        echo "demo: generating the most visited cities..."
        $executable $copied_file 1 0 "temp/t_tmp.csv" "temp/s_tmp.csv"
        echo "demo: the most visited cities have been generated."

        sort --parallel 16 -t';' -k2nr "temp/t_tmp.csv" | head -$option_arg > "temp/t_tmp_sorted.csv"

        gnuplot ./progc/gnuplot/t_plot.gnu

        echo "created: images/t_image.png"

        end_timer
      ;;
    "-s")
        option_start=0
        option_end=25

        # if the option has an argument, get it
        if [ $# -gt 1 ]; then
            # if the argument is a number, get it
            if [ $2 -eq $2 ] 2> /dev/null; then
                option_start=$2
                option_end=$((option_start + 25))
                shift
            fi
        fi

        # if the option has an argument, get it
        if [ $# -gt 1 ]; then
            # if the argument is a number, get it
            if [ $3 -eq $3 ] 2> /dev/null; then
                option_end=$2
                shift
            fi
        fi

        line_count=$(wc -l < $copied_file)
        tail_count=$(($line_count - $option_start))
        head_count=$(($option_end - $option_start))

        if [ $tail_count -lt 0 ]; then
            >&2 echo "error: the start value is greater than the number of lines in the file."
            exit_properly
        fi

        if [ $head_count -lt 0 ]; then
            >&2 echo "error: the end value is greater than the number of lines in the file."
            exit_properly
        fi

        # minimum gap between start and end
        if [ $head_count -lt 2 ]; then
            >&2 echo "error: the gap between start and end must be at least 2."
            exit_properly
        fi

        # check if end is greater than start
        if [ $option_end -lt $option_start ]; then
            >&2 echo "error: the end value must be greater than the start value."
            exit_properly
        fi

        if [ $compiled -eq 0 ]; then
            test_executable
        fi

        start_timer
        echo "demo: generating the routes length..."
        $executable $copied_file 0 1 "temp/t_tmp.csv" "temp/s_tmp.csv"
        echo "demo: the routes length have been generated."

        sort --parallel 16 -t';' -k1nr "temp/s_tmp.csv" | tail -$tail_count | head -$head_count > "temp/s_tmp_sorted.csv"

        gnuplot ./progc/gnuplot/s_plot.gnu

        echo "created: images/s_image.png"

        end_timer
      ;;
    ?)
        >&2 echo "error: unknown option $1"
        exit_properly
        ;;
    esac
    shift
done

exit_properly
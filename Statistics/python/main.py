import statistics as st
import os
import sys
import itertools

# Default files
dirname = os.path.dirname(__file__)

input = os.path.join(dirname, "../csv/df_surf.csv")
output = os.path.join(dirname, "../output.txt")

# User-defined files
if (len(sys.argv) >= 3):
    input = sys.argv[1]
    output = sys.argv[2]

stat = st.Statistics(input, output)

first_column = next(stat.__iter__())

print("The following tests apply complete statistical analysis on each column.")
for col in stat : 

    # Mean of each column
    try:
        print(f"Mean of {col[0]}: {stat.mean(col[0])}")
        stat.write_mean(col[0])
    except Exception as e: print(f"{type(e).__name__}: {e}")

    # Median of each column
    try:
        print(f"Median of {col[0]}: {stat.median(col[0])}")
        stat.write_median(col[0])
    except Exception as e: print(f"{type(e).__name__}: {e}")

    # Variance of each column
    try:
        print(f"Variance of {col[0]}: {stat.variance(col[0])}")
        stat.write_variance(col[0])
    except Exception as e: print(f"{type(e).__name__}: {e}")
    
    # Standard deviation of each column
    try:
        print(f"Standard deviation of {col[0]}: {stat.sdeviation(col[0])}")
        stat.write_sdeviation(col[0])
    except Exception as e: print(f"{type(e).__name__}: {e}")

    # Number of occurrences of first element of each column
    try:
        print(f"Number of occurrences of element \"{col[1][0]}\" of {col[0]}: {stat.count(col[1][0], col[0])}")
        stat.write_count(col[1][0], col[0])
    except Exception as e: print(f"{type(e).__name__}: {e}")

    # Frequency of first element of each column
    try:
        print(f"Frequency of element \"{col[1][0]}\" of {col[0]}: {stat.frequency(col[1][0], col[0])}")
        stat.write_frequency(col[1][0], col[0])
    except Exception as e: print(f"{type(e).__name__}: {e}")

    # Correlation of first column with each column
    try:
        print(f"Correlation between {first_column[0]} and {col[0]}: {stat.correlation(first_column[0], col[0])}")
        stat.write_correlation(first_column[0], col[0])
    except Exception as e: print(f"{type(e).__name__}: {e}")

try:
    new_stat = stat.classification(first_column[1][0], first_column[0])
    print("A new Statistics object with only rows containing the first element of first column has been created:")
    print(new_stat)
except Exception as e: print(f"{type(e).__name__}: {e}")

print("The following tests compare the execution time between python-only methods and the python/C++ hybrid approach")

# These names refer to our default csv file
numerical_col_name = next(itertools.islice(stat.__iter__(), 2, None))[0]
categorical_col_name = next(itertools.islice(stat.__iter__(), 1, None))[0]

try:
    stat.timed_mean(numerical_col_name)
    stat.mean_py(numerical_col_name)
    stat.timed_write_mean(numerical_col_name)
    stat.write_mean_py(numerical_col_name, output)
except Exception as e: print(f"{type(e).__name__}: {e}")

print("As you can see, the Python-only methods are slower than the hybrid ones. Both methods need to parse all the values in the column, but since the actual mean is implemented in C, the delay is caused by the column parsing.")

# Histogram of the frequency of elements of the selected column.
print(f"Plotting histogram of column {numerical_col_name}")
stat.plot_column(numerical_col_name) # Numerical
print(f"Plotting histogram of column {categorical_col_name}")
stat.plot_column(categorical_col_name) # Categorical

# Plots of the correlation between two columns
try:
    print(f"Plotting scatter plot between columns {first_column[0]} and {numerical_col_name}")
    stat.plot_correlation(first_column[0], numerical_col_name)
    print(f"Plotting scatter plot between columns {numerical_col_name} and itself")
    stat.plot_correlation(numerical_col_name, numerical_col_name)
    print(f"Plotting scatter plot between columns {categorical_col_name} and {numerical_col_name}")
    stat.plot_correlation(categorical_col_name, numerical_col_name) # This will raise exception
except Exception as e: print(f"{type(e).__name__}: {e}")

import build.statistics as bst
import time
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

class Statistics(bst.Statistics):
    # Constructor
    def __init__(self, input, output):
        super().__init__(input, output)
    
    # Timer decorator
    def timer(my_function):
        def wrapper(*args):
            t1 = time.time()
            result = my_function(*args)
            t2 = time.time()
            print(f"{my_function.__name__} ran in {t2 - t1} sec")
            return result
        return wrapper

    # Timed methods
    @timer
    def timed_mean(self, col_name) :
        return self.mean(col_name)

    @timer
    def timed_write_mean(self, col_name) :
        self.write_mean(col_name)
    
    @timer
    def timed_median(self, col_name) :
        return self.median(col_name)

    @timer
    def timed_write_median(self, col_name) :
        self.write_median(col_name)
    
    @timer
    def timed_variance(self, col_name) :
        return self.variance(col_name)

    @timer
    def timed_write_variance(self, col_name) :
        self.write_variance(col_name)
    
    @timer
    def timed_sdeviation(self, col_name) :
        return self.sdeviation(col_name)

    @timer
    def timed_write_sdeviation(self, col_name) :
        self.write_sdeviation(col_name)
    
    @timer
    def timed_count(self, given_el, col_name) :
        return self.count(given_el, col_name)

    @timer
    def timed_write_count(self, given_el, col_name) :
        self.write_count(given_el, col_name)
    
    @timer
    def timed_frequency(self, given_el, col_name) :
        return self.frequency(given_el, col_name)

    @timer
    def timed_write_frequency(self, given_el, col_name) :
        self.write_frequency(given_el, col_name)
    
    @timer
    def timed_correlation(self, col_name_1, col_name_2) :
        return self.correlation(col_name_1, col_name_2)

    @timer
    def timed_write_correlation(self, col_name_1, col_name_2) :
        self.write_correlation(col_name_1, col_name_2)
    
    @timer
    def timed_classification(self, given_el, col_name) :
        return self.classification(given_el, col_name)
    
    # Utility method to extract a column as a tuple of (name, values)
    def get_col(self, col_name):
        for col in self:
            if col[0] == col_name:
                return col

    # Python-only methods for performance tests
    @timer
    def mean_py(self, col_name):
        column = self.get_col(col_name)[1]
        while "" in column: column.remove("") # Deal with empty values
        if all([(isinstance(el, float) or isinstance(el, int)) for el in column]):
            np.asarray(column)
            return np.mean(column)
        else:
            raise TypeError("Column contains strings, can't calculate mean.")

    @timer
    def write_mean_py(self, col_name, output_file):
        with open(output_file, "a") as file:
            file.write(f"Mean of {col_name} = {self.mean_py(col_name)}")

    # Plotting methods
    def plot_column(self, col_name):
        column = self.get_col(col_name)[1]
        while "" in column: column.remove("") # Deal with empty values
        sns.histplot(column)
        plt.title(f"Frequency of values in {col_name}")
        plt.xlabel("Values")
        plt.ylabel("Frequency")
        try:
            plt.axvline(self.mean(col_name), color='red', linestyle='solid', linewidth=2, label='Mean')
            plt.axvline(self.median(col_name), color='blue', linestyle='dotted', linewidth=2, label='Median')
            plt.axvline(self.variance(col_name), color='green', linestyle='dashdot', linewidth=2, label='Variance')
            plt.axvline(self.sdeviation(col_name), color='purple', linestyle='dashed', linewidth=2, label='Standard Deviation')
            plt.legend()
        except ValueError: # --> values are categorical, we simply plot the histogram.
            pass
        plt.show()
    
    def plot_correlation(self, col_name_1, col_name_2):
        correlation = self.correlation(col_name_1, col_name_2)
        column_1 = self.get_col(col_name_1)[1]
        column_2 = self.get_col(col_name_2)[1]
        for i in range(len(column_1)-1, -1, -1): # Backwards iteration to delete missing values
            if column_1[i] == "" or column_2[i] == "":
                del column_1[i]
                del column_2[i]
        sns.scatterplot(x=column_1, y=column_2)
        plt.suptitle(f"Scatterplot of correlation between {col_name_1} and {col_name_2}")
        plt.title(f"Correlation = {correlation: .6f}")
        plt.xlabel(col_name_1)
        plt.ylabel(col_name_2)
        plt.axline((0, 0), (1, 1), color='red') # Visual representation of a theoretical correlation = 1
        plt.show()


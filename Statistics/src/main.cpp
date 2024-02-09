#include "../include/statistics.hpp"

#include <fstream>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    // Default files
    std::string input = "csv/df_surf.csv";
    std::string output = "output.txt";

    // User-defined files
    if (argc >= 3)
    {
        input = argv[1];
        output = argv[2];
    }
    sctoolbox::Statistics stat(input, output);

    // Test applies complete statistical analysis on each column.
    for (auto it = stat.begin(); it != stat.end(); ++it)
    {
        // Mean of each column
        try
        {
            std::cout << "Mean of " << it->first << ": " << stat.mean(it->first) << std::endl;
            stat.write_mean(it->first);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }

        // Median of each column
        try
        {
            std::cout << "Median of " << it->first << ": " << stat.median(it->first) << std::endl;
            stat.write_median(it->first);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }

        // Variance of each column
        try
        {
            std::cout << "Variance of " << it->first << ": " << stat.variance(it->first) << std::endl;
            stat.write_variance(it->first);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }

        // Standard deviation of each column
        try
        {
            std::cout << "Standard deviation of " << it->first << ": " << stat.sdeviation(it->first) << std::endl;
            stat.write_sdeviation(it->first);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }

        // Number of occurrences of first element of each column
        try
        {
            std::cout << "Number of occurences of " << it->first << ": " << stat.count(it->second[0], it->first) << std::endl;
            stat.write_count(it->second[0], it->first);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }

        // Frequency of first element of each column
        try
        {
            std::cout << "Frequency of " << it->first << ": " << stat.frequency(it->second[0], it->first) << std::endl;
            stat.write_frequency(it->second[0], it->first);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }

        // Correlation of first column with each column
        try
        {
            std::cout << "Correlation between " << stat.begin()->first << " and " << it->first << ": " << stat.correlation(stat.begin()->first, it->first) << std::endl;
            stat.write_correlation(stat.begin()->first, it->first);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }
        std::cout << std::endl;
    }

    try
    {
        sctoolbox::Statistics new_stat = stat.classification(stat.begin()->second[0], stat.begin()->first);
        std::cout << "A new Statistics object with only rows containing the first element of first column has been created:" << std::endl;
        new_stat.print();
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}
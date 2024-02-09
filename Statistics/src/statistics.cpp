#include "../include/statistics.hpp"
#include <boost/tokenizer.hpp>

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace sctoolbox;

// Checks if an element is a number
bool is_number(const std::string &s)
{
    char *end = nullptr;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
}

Statistics::Statistics(const std::string input_file, const std::string output_file) : data(new data_type), output_file(output_file)
{
    using namespace boost;
    std::ifstream file(input_file);
    if (file.is_open())
    {
        std::string line;
        std::getline(file, line); // First line contains titles of columns
        tokenizer<escaped_list_separator<char>> tokens(line);
        for (const auto &t : tokens) // Titles of columns are the map's keys
        {
            (*data)[t];
        }
        while (std::getline(file, line))
        {
            tokenizer<escaped_list_separator<char>> tokens(line);
            tokenizer<escaped_list_separator<char>>::iterator it = tokens.begin();
            for (auto &[_, v] : *data) // Each map's value is filled with the corresponding column
            {
                if (is_number(*it)) // Check to store as either double or string
                {
                    v.push_back(std::stod(*it));
                }
                else
                {
                    v.push_back(*it);
                }
                ++it;
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "Failed to open the file." << std::endl;
    }
}

// Additional constructor used by the classification method
Statistics::Statistics(const data_type input_data, const std::string output_file) : data(new data_type), output_file(output_file) {
    // Copy everything from input_data to the data_type data points to
    for (const auto &[k, v] : input_data)
        {
            (*data)[k] = v;
        }
}

// Prints the entire map
void Statistics::print() const
{
    for (const auto &pair : *data)
    {
        std::cout << pair.first << ": ";
        for (const auto &el : pair.second)
        {
            // Lambda function evaluates type of el at runtime
            std::visit([](const auto &x)
                       { std::cout << x; },
                       el);
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

// Prints the desired column
void Statistics::print(const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        std::cout << col_name << ": ";
        for (const auto &el : data->at(col_name))
        {
            // Lambda function evaluates type of el at runtime
            std::visit([](const auto &x)
                       { std::cout << x; },
                       el);
            std::cout << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

double Statistics::mean(const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        double m = 0.0;
        unsigned int i = 0;
        for (const auto &el : data->at(col_name))
        {
            try
            {
                m += std::get<double>(el); // Try to get element as double
                ++i;
            }
            catch (const std::bad_variant_access &e)
            {
                if (std::get<std::string>(el) == "") // Skip missing values
                {
                    continue;
                }
                else // Stop if value is categorical
                {
                    throw std::invalid_argument("Column contains strings, can't calculate mean.");
                }
            }
        }
        if (i > 0) // Handles empty columns
        {
            m /= i; // Mean evaluation
        }
        return m;
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

// Writes mean in output file
void Statistics::write_mean(const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        const double m = mean(col_name);
        std::ofstream file(output_file, std::ios_base::app);
        if (file.is_open())
        {
            file << "Mean of " << col_name << " = " << m << std::endl;
            file.close();
        }
        else
        {
            std::cerr << "Failed to open the file." << std::endl;
        }
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

double Statistics::median(const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        std::vector<double> sorted_values;
        for (const auto &el : data->at(col_name))
        {
            try
            {
                sorted_values.push_back(std::get<double>(el)); // Try to get element as double
            }
            catch (const std::bad_variant_access &e)
            {
                if (std::get<std::string>(el) == "") // Skip missing values
                {
                    continue;
                }
                else // Stop if value is categorical
                {
                    throw std::invalid_argument("Column contains strings, can't calculate median.");
                }
            }
        }
        std::sort(sorted_values.begin(), sorted_values.end());

        const size_t size = sorted_values.size();
        if (size == 0) // Handles empty columns
        {
            return 0;
        }
        if (size % 2 == 0) // Median evaluation
        {
            // If the number of elements is even, average the middle two numbers.
            const size_t mid = size / 2;
            return (sorted_values[mid - 1] + sorted_values[mid]) / 2.0;
        }
        else
        {
            // If the number of elements is odd, return the middle number.
            return sorted_values[size / 2];
        }
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

// Writes median in output file
void Statistics::write_median(const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        const double m = median(col_name);
        std::ofstream file(output_file, std::ios_base::app);
        if (file.is_open())
        {
            file << "Median of " << col_name << " = " << m << std::endl;
            file.close();
        }
        else
        {
            std::cerr << "Failed to open the file." << std::endl;
        }
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

double Statistics::variance(const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        const double m = mean(col_name);
        double variance = 0.0;
        unsigned int i = 0;

        for (const auto &el : data->at(col_name))
        {
            try
            {
                variance += (std::get<double>(el) - m) * (std::get<double>(el) - m); // Try to get element as double
                ++i;
            }
            catch (const std::bad_variant_access &e)
            {
                if (std::get<std::string>(el) == "") // Skip missing values
                {
                    continue;
                }
                else // Stop if value is categorical
                {
                    throw std::invalid_argument("Column contains strings, can't calculate median.");
                }
            }
        }

        if (i > 0) // Handles empty columns
        {
            variance /= i; // Variance evaluation
        }
        return variance;
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

// Writes variance in output file
void Statistics::write_variance(const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        const double v = variance(col_name);
        std::ofstream file(output_file, std::ios_base::app);
        if (file.is_open())
        {
            file << "Variance of " << col_name << " = " << v << std::endl;
            file.close();
        }
        else
        {
            std::cerr << "Failed to open the file." << std::endl;
        }
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

double Statistics::sdeviation(const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        const double v = variance(col_name);
        return std::sqrt(v); // Standard deviation is square root of variance
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

// Writes standard deviation in output file
void Statistics::write_sdeviation(const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        const double sd = sdeviation(col_name);
        std::ofstream file(output_file, std::ios_base::app);
        if (file.is_open())
        {
            file << "Standard deviation of " << col_name << " = " << sd << std::endl;
            file.close();
        }
        else
        {
            std::cerr << "Failed to open the file." << std::endl;
        }
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

unsigned int Statistics::count(const std::variant<double, std::string> given_el, const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        unsigned int c = 0;
        for (const auto &el : data->at(col_name))
        {
            if (given_el == el)
            {
                ++c;
            }
        }
        return c;
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

// Writes number of occurrences in output file
void Statistics::write_count(const std::variant<double, std::string> given_el, const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        const unsigned int c = count(given_el, col_name);
        std::ofstream file(output_file, std::ios_base::app);
        if (file.is_open())
        {
            file << "Number of occurrences of \"";
            // Lambda function evaluates type of el at runtime
            std::visit([&file](const auto &x)
                       { file << x; },
                       given_el);
            file << "\" in " << col_name << " is " << c << std::endl;
            file.close();
        }
        else
        {
            std::cerr << "Failed to open the file." << std::endl;
        }
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

double Statistics::frequency(const std::variant<double, std::string> given_el, const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        // Frequency is number of occurrences / number of elements
        return count(given_el, col_name) / (double)data->at(col_name).size();
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

// Writes frequency in output file
void Statistics::write_frequency(const std::variant<double, std::string> given_el, const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        const double f = frequency(given_el, col_name);
        std::ofstream file(output_file, std::ios_base::app);
        if (file.is_open())
        {
            file << "Frequency of \"";
            // Lambda function evaluates type of el at runtime
            std::visit([&file](const auto &x)
                       { file << x; },
                       given_el);
            file << "\" in " << col_name << " is " << f << std::endl;
            file.close();
        }
        else
        {
            std::cerr << "Failed to open the file." << std::endl;
        }
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

// Computes correlation between two columns of numerical values (taken from ex session 3 exercise 5)
// Missing values are simply ignored (Complete Case Analysis)
double Statistics::correlation(const std::string col_name_1, const std::string col_name_2) const
{
    // Check if columns exist
    if (data->find(col_name_1) != data->end() && data->find(col_name_2) != data->end())
    {
        const unsigned int size = data->at(col_name_1).size();

        // Number of pairs considered
        unsigned int n = 0;

        // Calculate the means
        const double m1 = mean(col_name_1);
        const double m2 = mean(col_name_2);

        // Calculate the standard deviations
        const double sd1 = sdeviation(col_name_1);
        const double sd2 = sdeviation(col_name_2);

        // Calculate the sum of the cross-products of differences
        double sum_cross_products = 0.0;

        for (unsigned int i = 0; i < size; ++i)
        {
            try
            {
                const double diff1 = std::get<double>(data->at(col_name_1)[i]) - m1;
                const double diff2 = std::get<double>(data->at(col_name_2)[i]) - m2;
                sum_cross_products += diff1 * diff2;
                ++n;
            }
            catch (const std::bad_variant_access &e)
            {
                // At least one of the values is missing, so we skip the pair
                // The categorical case is already handled in the mean calculation
                continue;
            }
        }

        // Calculate the Pearson correlation coefficient
        return sum_cross_products / (n * sd1 * sd2);
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

// Writes correlation in output file
void Statistics::write_correlation(const std::string col_name_1, const std::string col_name_2) const
{
    // Check if columns exist
    if (data->find(col_name_1) != data->end() && data->find(col_name_2) != data->end())
    {
        const double c = correlation(col_name_1, col_name_2);
        std::ofstream file(output_file, std::ios_base::app);
        if (file.is_open())
        {
            file << "Correlation between \"" << col_name_1 << "\" and \"" << col_name_2 << "\" is " << c << std::endl;
            file.close();
        }
        else
        {
            std::cerr << "Failed to open the file." << std::endl;
        }
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}

// Returns a Statistics object with data comprised of only the rows that contain given_el in col_name
Statistics Statistics::classification(const std::variant<double, std::string> given_el, const std::string col_name) const
{
    if (data->find(col_name) != data->end()) // Check if column exists
    {
        data_type result_data;
        for (const auto &[k, _] : *data)
        {
            result_data[k]; // Copy keys in result_data
        }

        //For each element of the column
        for (unsigned int i = 0; i < data->at(col_name).size(); ++i)
        {
            // Check if it's a match
            if (given_el == data->at(col_name)[i])
            {
                // Fill corresponding row
                for (const auto &[k, _] : *data)
                    result_data.at(k).push_back(data->at(k)[i]);
            }
        }
        return Statistics(result_data, output_file);
    }
    else
    {
        throw std::invalid_argument("Column name not found.");
    }
}
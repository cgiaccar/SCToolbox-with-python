#ifndef STATISTICS_HPP_
#define STATISTICS_HPP_

#include <optional>
#include <variant>
#include <map>
#include <iterator>
#include <string>
#include <vector>
#include <memory>
#include "fifo_map.hpp"

namespace sctoolbox
{
    // data_type is data's type. Using typename here for convenience
    using data_type = typename nlohmann::fifo_map<std::string, std::vector<std::variant<double, std::string>>>;
    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;
    using reverse_iterator = typename data_type::reverse_iterator;
    using const_reverse_iterator = typename data_type::const_reverse_iterator;

    class Statistics
    {
    public:
        Statistics(const std::string input_file, const std::string output_file);

        Statistics(const data_type input_data, const std::string output_file);

        void print() const;
        void print(const std::string col_name) const;

        // Iterators and constant iterators, reverse and constant
        iterator begin() { return data->begin(); }
        iterator end() { return data->end(); }
        const_iterator begin() const { return data->begin(); }
        const_iterator end() const { return data->end(); }
        const_iterator cbegin() const { return data->cbegin(); }
        const_iterator cend() const { return data->cend(); }
        reverse_iterator rbegin() { return data->rbegin(); }
        reverse_iterator rend() { return data->rend(); }
        const_reverse_iterator rbegin() const { return data->rbegin(); }
        const_reverse_iterator rend() const { return data->rend(); }
        const_reverse_iterator crbegin() const { return data->crbegin(); }
        const_reverse_iterator crend() const { return data->crend(); }

        // Utilities for statistical analysis
        double mean(const std::string col_name) const;
        void write_mean(const std::string col_name) const;

        double median(const std::string col_name) const;
        void write_median(const std::string col_name) const;

        double variance(const std::string col_name) const;
        void write_variance(const std::string col_name) const;

        double sdeviation(const std::string col_name) const;
        void write_sdeviation(const std::string col_name) const;

        unsigned int count(const std::variant<double, std::string> given_el, const std::string col_name) const;
        void write_count(const std::variant<double, std::string> given_el, const std::string col_name) const;

        double frequency(const std::variant<double, std::string> given_el, const std::string col_name) const;
        void write_frequency(const std::variant<double, std::string> given_el, const std::string col_name) const;

        double correlation(const std::string col_name_1, const std::string col_name_2) const;
        void write_correlation(const std::string col_name_1, const std::string col_name_2) const;

        Statistics classification(const std::variant<double, std::string> given_el, const std::string col_name) const;

    private:
        std::unique_ptr<data_type> data;
        std::string output_file;
    };
}

#endif
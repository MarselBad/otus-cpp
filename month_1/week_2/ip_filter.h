#ifndef IP_FILTER_IP_FILTER_H
#define IP_FILTER_IP_FILTER_H

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

//! Splits given by given character
/*!
 * \param str string for splitting
 * \param d character to split given string
 * \return vector of substrings that resided between given character in original string
 *
 * Example:
 * \code
 *
 * auto sub_str_vec = split("",  '.')     // -> [""]
 * auto sub_str_vec = split("11", '.')    // -> ["11"]
 * auto sub_str_vec = split("..", '.')    // -> ["", "", ""]
 * auto sub_str_vec = split("11.", '.')   // -> ["11", ""]
 * auto sub_str_vec = split(".11", '.')   // -> ["", "11"]
 * auto sub_str_vec = split("11.22", '.') // -> ["11", "22"]
 *
 * \endcode
*/
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

//! IPv4 address represented by 4 bytes
using ipv4_t = std::array<int, 4>;
//! Pool of IPv4 addresses represented by 4 bytes
using ipv4_vec = std::vector<ipv4_t>;

//! Given IPv4 address represented by 4 bytes convert it into 32bit unsigned integer number.
uint32_t ipv4_to_uint(const ipv4_t& a){
    uint32_t addr = 0;
    addr = a[0] << 24;
    addr |= a[1] << 16;
    addr |= a[2] << 8;
    addr |= a[3];
    return addr;
}

//! Given 32bit unsigned integer number convert it into IPv4 address represented by 4 bytes.
ipv4_t uint_to_ipv4(const uint32_t& v){
    return {uint8_t(v>>24&255), uint8_t(v>>16&255), uint8_t(v>>8&255), uint8_t(v&255)};
}

//! Prints pool of IPv4 addresses in rows
void print_ip_pool(const ipv4_vec& ip_pool){
    for(const auto& a: ip_pool)
        std::cout << a[0] << '.' << a[1] << '.' << a[2] << '.' << a[3] << std::endl;
}

//! Sort given pool of IPv4 addresses
/*!
 * \param ip_pool pool of IPv4 addresses
 * \param ascending flag indicating whether addresses must be sorted in ascending or descending order
 *
 * Example:
 * \code
 *
 * ipv4_vec ip_pool = {
 *   {1,2,3,4},
 *   {3,2,4,5},
 *   {3,2,1,6},
 *   {4,7,2,7},
 *   {1,2,3,1},
 * },
 *
 * // sorted ascending
 * sort(ip_pool, true);
 *
 * // print_ip_pool(ip_pool);  // assume that it prints addressed by line
 * // Output:
 * // 1.2.3.1
 * // 1.2.3.4
 * // 3.2.1.6
 * // 3.2.4.5
 * // 4.7.2.7
 *
 * // sorted descending
 * sort(ip_pool, true);
 *
 * // print_ip_pool(ip_pool);  // assume that it prints addressed by line
 * // Output:
 * // 4.7.2.7
 * // 3.2.4.5
 * // 3.2.1.6
 * // 1.2.3.4
 * // 1.2.3.1
 *
 * \endcode
*/
void sort(ipv4_vec& ip_pool, bool ascending=true) {
    if (ascending)
        std::sort(ip_pool.begin(), ip_pool.end(), std::less<ipv4_t>());
    else
        std::sort(ip_pool.begin(), ip_pool.end(), std::greater<ipv4_t>());
}

//! Filter given pool of IPv4 addresses by mask and filter values
/*!
 * \tparam Args type of the arguments
 * \param ip_pool pool of IPv4 addresses
 * \param positions mask for filtering, represented as 4 bytes, each byte specify whether corresponding IPv4 byte
 *                  must be checked by filter, 0 - byte will not be checked, otherwise will be checked
 *                  1.0.1.0 -> checked.ignored.checked.ignored
 * \param args values for address filtering
 * \return pool of IPv4 addresses filtered according to given mask and filter
 *
 * Example:
 * \code
 * auto filtered_pool = filter(
 *   {
 *     {1,2,3,4},
 *     {3,1,4,5},
 *     {3,2,1,6},
 *     {4,7,2,7},
 *     {5,2,3,1},
 *   },            // pool of IPv4 addresses
 *   {1,0,1,0},    // mask for each byte
 *   1, 2          // filter values
 * );
 *
 * // print_ip_pool(filtered_pool);  // assume that it prints addressed by line
 * // Output:
 * // 1.2.3.4
 * // 3.2.1.6
 * // 4.7.2.7
 *
 * \endcode
*/
template<class ...Args>
ipv4_vec filter(const ipv4_vec& ip_pool, const ipv4_t& positions, Args... args) {
    ipv4_vec ip_pool_filtrd;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(ip_pool_filtrd),
                 [positions, args...](const ipv4_t& a) {
                     // check each filter value
                     for(auto v: {args...}) {
                         // check each address byte
                         for (std::size_t i = 0; i < positions.size(); ++i) {
                             // if current byte must be filtered and address byte accepted by filter
                             if (positions[i] && a[i] == int(v))
                                 return true;
                         }
                     }
                     return false;
                 });
    return ip_pool_filtrd;
}

//! Filter given pool of IPv4 addresses by mask and filter values
/*!
 * \param ip_pool pool of IPv4 addresses
 * \param positions mask for filtering, represented as 4 bytes, each byte specify whether corresponding IPv4 byte
 *                  must be checked by filter, 0 - byte will not be checked, otherwise will be checked
 *                  1.0.1.0 -> checked.ignored.checked.ignored
 * \param filter_vals values for address filtering
 * \return pool of IPv4 addresses filtered according to given mask and filter
 *
 * Example:
 * \code
 * auto filtered_pool = filter(
 *   {
 *     {1,2,3,4},
 *     {3,1,4,5},
 *     {3,2,1,6},
 *     {4,7,2,7},
 *     {5,2,3,1},
 *   },            // pool of IPv4 addresses
 *   {1,0,1,0},    // mask for each byte
 *   {1, 2}        // filter values
 * );
 *
 * // print_ip_pool(filtered_pool);  // assume that it prints addressed by line
 * // Output:
 * // 1.2.3.4
 * // 3.2.1.6
 * // 4.7.2.7
 *
 * \endcode
*/
ipv4_vec filter2(const ipv4_vec& ip_pool, const ipv4_t& positions, const std::vector<int>& filter_vals) {
    ipv4_vec ip_pool_filtrd;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(ip_pool_filtrd),
                 [positions, filter_vals](const ipv4_t& a) {
                     // check each filter val
                     for(auto v: filter_vals) {
                         // check each address byte
                         for (std::size_t i = 0; i < positions.size(); ++i) {
                             // if current byte must be filtered and address byte accepted by filter
                             if (positions[i] && a[i] == int(v))
                                 return true;
                         }
                     }
                     return false;
                 });
    return ip_pool_filtrd;
}

//! Filter given pool of IPv4 addresses by filter values
/*!
 * \param ip_pool pool of IPv4 addresses
 * \param filter_vals filter, represented as 4 bytes, each byte specify value for filtering IPv4 byte
 *                    all specified filter values must occur in address, if -1 - then byte will be ignored
 *                    by filter, 3.2.-1.-1 -> checked.checked.ignored.ignored
 * \return pool of IPv4 addresses filtered according to given filter
 *
 * Example:
 * \code
 * auto filtered_pool = filter_positions(
 *   {
 *     {1,2,3,4},
 *     {3,1,4,5},
 *     {3,2,1,6},
 *     {4,7,2,7},
 *     {3,2,3,1},
 *   },           // pool of IPv4 addresses
 *   {3,2,-1,-1}  // filter
 * );
 *
 * // print_ip_pool(filtered_pool);  // assume that it prints addressed by line
 * // Output:
 * // 3.2.1.6
 * // 3.2.3.1
 *
 * \endcode
*/
ipv4_vec filter_positions(const ipv4_vec& ip_pool, const ipv4_t& filter_vals) {
    ipv4_vec ip_pool_filtrd;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(ip_pool_filtrd),
                 [filter_vals](const ipv4_t& a) {
                     // check each byte if it must be filtered and if all bytes that must be filtered
                     // are accepted by corresponding filter values
                     return ((filter_vals[0] <= 0 || filter_vals[0] == a[0]) &&
                             (filter_vals[1] <= 0 || filter_vals[1] == a[1]) &&
                             (filter_vals[2] <= 0 || filter_vals[2] == a[2]) &&
                             (filter_vals[3] <= 0 || filter_vals[3] == a[3]));
                 });
    return ip_pool_filtrd;
}

#endif //IP_FILTER_IP_FILTER_H

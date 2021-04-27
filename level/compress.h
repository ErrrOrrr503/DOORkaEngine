#ifndef DE_COMP_H
#define DE_COMP_H
//! \file compress.h

#include <vector>
#include <string>
#include <cstring>
#include <unordered_map>

/*! \brief Return codes for compress() function
 * \param COMP_OK Compression was successful
 * \param COMP_ERR_ALLOC Could not allocate memory (std::bad_alloc is also thrown at this case)
 * \param COMP_ERR_DECODE The data is not a valid compressed file and cannot be decoded
 */
enum return_codes {
    COMP_OK = 0,
    COMP_ERR_ALLOC,
    COMP_ERR_DECODE
};

//! Reserved constant for comparision (костыль)
const std::string EMPTY_STR = std::string();
//! Dictionary size that corresponds to 12-bit code word
const size_t TABLE_SIZE = 4096;
//! Type for a code word, represents position of an encoded sequence in a dictionary
typedef unsigned short pos_t;

/*!
 * Compress an array of data using LZW algorithm
 * \param input Input array of data
 * \param input_size Size of the array in bytes
 * \return Vector of compressed data stored in 16-bit code words
 * \details May throw any of standard std::vector and std::string exceptions.
 * The higher 4 bits of each 16-bit code word are insignificant, use pack() function to pack them densely.
 */
std::vector<pos_t> compress(const char* input, size_t input_size);
int decompress(const pos_t* input, size_t input_size, char* result);
size_t pack(std::vector<pos_t>& input, unsigned char** output);
pos_t* unpack(size_t codon_count, const unsigned char* packed);

inline void unpack_pair(const unsigned char* in_buf, pos_t* out_buf)
{
    auto out_bytes = (unsigned char*)out_buf;
    out_bytes[0] = in_buf[0];
    out_bytes[1] = in_buf[1] >> 4u;
    out_bytes[2] = (unsigned char)(in_buf[1] << 4u) + (unsigned char)(in_buf[2] >> 4u);
    out_bytes[3] = in_buf[2] & 0x0F;
}

inline void pack_pair(const pos_t* in_buf, unsigned char* out_buf)
{
    auto in_bytes = (const unsigned char*)in_buf;
    out_buf[0] = in_bytes[0];
    out_buf[1] = (unsigned char)(in_bytes[1] << 4u) + (unsigned char)(in_bytes[2] >> 4u);
    out_buf[2] = (unsigned char)(in_bytes[2] << 4u) + (unsigned char)(in_bytes[3] & 0x0F);
}

#endif // DE_COMP_H

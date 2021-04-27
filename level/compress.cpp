#include "compress.h"

std::vector<pos_t> compress(const char* input, size_t input_size)
{
    std::vector<pos_t> result;
    size_t dict_size = 256;
    std::unordered_map<std::string, pos_t> dict;
    for (pos_t i = 0; i < dict_size; i++)
        dict[std::string(1, i)] = i;
    std::string buffer, bc;
    for (size_t i = 0; i < input_size; i++) {
        char c = input[i];
        bc = buffer + c;
        if (dict.count(bc) != 0) {
            buffer = bc;
        } else {
            result.push_back(dict[buffer]);
            if (dict_size < TABLE_SIZE)
                dict[bc] = dict_size++;
            buffer = std::string(1, c);
        }
    }
    if (!buffer.empty())
        result.push_back(dict[buffer]);
    return result;
}

int decompress(const pos_t* input, size_t input_size, char* result)
{
    size_t dict_size = 256;
    std::string** dict =  (std::string**)calloc(TABLE_SIZE, sizeof(std::string*));
    if (dict == nullptr)
        return COMP_ERR_ALLOC;
    try {
        for (pos_t i = 0; i < dict_size; i++)
            dict[i] = new std::string(1, i);
    } catch(std::bad_alloc) {
        for (pos_t i = 0; i < dict_size; i++)
            delete dict[i];
        free(dict);
        return COMP_ERR_ALLOC;
    }
    std::string w(1, input[0]);
    result[0] = input[0];
    size_t result_offset = 1;
    std::string buffer;
    for (size_t i = 1; i < input_size; i++) {
        pos_t code = input[i];
        if (dict[code] != nullptr) {
            buffer = *dict[code];
        } else if (code == dict_size) {
            buffer = w + w[0];
        } else {
            for (size_t i = 0; i < dict_size; i++)
                delete dict[i];
            free(dict);
            return COMP_ERR_DECODE;
        }
        memcpy(result + result_offset, buffer.c_str(), buffer.size()); // result += buffer;
        result_offset += buffer.size();
        try {
            if (dict_size < TABLE_SIZE) {
                dict[dict_size++] = new std::string(w + buffer[0]);
            }
        } catch(std::bad_alloc) {
            for (pos_t i = 0; i < dict_size; i++)
                delete dict[i];
            free(dict);
            return COMP_ERR_ALLOC;
        }
        w = buffer;
    }
    for (size_t i = 0; i < dict_size; i++)
        delete dict[i];
    free(dict);
    return COMP_OK;
}

size_t pack(std::vector<pos_t>& input, unsigned char** output)
{
    // We will write down an empty triplet
    // It happens when input.size() is an odd number.
    int odd = false;
    if (input.size() % 2) {
        input.push_back(0);
        odd = true;
    }
    size_t out_size = input.size() / 2 * 3;
    *output = new unsigned char[out_size];
    for (size_t i = 0, j = 0; i < input.size(); i += 2, j += 3)
        pack_pair(input.data() + i, (*output) + j);
    if (odd == true)
        input.pop_back();
    return out_size;
}

pos_t* unpack(size_t codon_count, const unsigned char* packed)
{
    pos_t* output = new pos_t[codon_count + 1];
    for (size_t i = 0, j = 0; i < codon_count; i += 2, j += 3)
        unpack_pair(packed + j, output + i);
    return output;
}

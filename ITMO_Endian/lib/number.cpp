#include "number.h"

// uint64_t Power(uint64_t num, uint64_t pow) {
//   uint64_t res_num = 1;

//   for (int i = 0; i < pow; ++i) {
//     res_num *= num;
//   }

//   return res_num;
// }

void LeftShift(uint239_t& value, uint64_t shift) {
  if (shift == 0) {
    return;
  }

  const auto kTotalDataBits = 245;
  const auto kInByteDataBits = 7;

  uint64_t original_shift = GetShift(value);
  uint8_t valid_shift = shift % kTotalDataBits;

  int first_byte = valid_shift / kInByteDataBits;
  int last_byte = 0;

  int lshift_bits = valid_shift % kInByteDataBits;
  int rshift_bits = kInByteDataBits - (lshift_bits % kInByteDataBits);

  uint239_t buffer = value;

  for (int j = first_byte; j < 34; ++j) {
    value.data[j - first_byte] = ((value.data[j] << lshift_bits) & 127) | (value.data[j + 1] >> rshift_bits);
      last_byte = j - first_byte + 1;
  }
  
  value.data[last_byte] =((value.data[34] << lshift_bits) & 127) | (buffer.data[0] >> rshift_bits);
  
  for (int j = 0; j < first_byte; ++j) {
    value.data[35 - first_byte + j] = ((buffer.data[j] << lshift_bits) & 127) | (buffer.data[j + 1] >> rshift_bits);
  }
  
  int index = 34;
  shift = (shift + original_shift) % (1ll << 35);

  while (shift > 0) {
    value.data[index] = value.data[index] | ((shift & 1) << kInByteDataBits);
    shift = shift >> 1;
    --index;
  }
  
  //   if (shift != 0) {
  //   uint64_t original_shift = GetShift(value);
  //   uint64_t valid_shift = shift % (kTotalDataBits);

  //   for (uint64_t i = 0; i < valid_shift; ++i) {
  //     uint8_t first_bit = (value.data[0] % 128) / 64;
  //     for (uint64_t j = 0; j < 34; ++j) {
  //       value.data[j] = (value.data[j] % 64) * 2 + ((value.data[j + 1] % 128) / 64);
  //     }
  //     value.data[34] = (value.data[34] % 64) * 2 + first_bit;
  //   }

  //   int j = 34;
  //   shift = (shift + original_shift) % (1ll << 35);

  //   while (shift > 0) {
  //     value.data[j] += (shift % 2) * 128;
  //     shift /= 2;
  //     --j;
  //   }
  // }
}

void RightShift(uint239_t& value, uint64_t shift) {
  if (shift == 0) {
    return;
  }

  const auto kTotalDataBits = 245;
  const auto kInByteDataBits = 7;

  uint64_t original_shift = GetShift(value);
  uint8_t valid_shift = shift % (kTotalDataBits);

  int first_byte = valid_shift / kInByteDataBits;
  int last_byte = 34;

  int lshift_bits = valid_shift % kInByteDataBits;
  int rshift_bits = kInByteDataBits - (lshift_bits % kInByteDataBits);

  uint239_t buffer = value;

  for (int j = 34; j > first_byte; --j) {
    value.data[j] = ((value.data[j - first_byte] & 127) >> lshift_bits) | ((value.data[j - 1 - first_byte] << rshift_bits) & 127);
    last_byte = j - 1;
  }

  value.data[last_byte] =((value.data[0] & 127) >> lshift_bits) | ((buffer.data[34] << rshift_bits) & 127);
  
  for (int j = first_byte-1; j >= 0; --j) {
    value.data[j] = ((buffer.data[34 - j] & 127) >> lshift_bits) | ((buffer.data[34 - j - 1] << rshift_bits) & 127);
  }

  int index = 34;
  shift = (original_shift - shift) % (1ll << 35);

  while (shift > 0) {
    value.data[index] = value.data[index] | ((shift & 1) << kInByteDataBits);
    shift = shift >> 1;
    --index;
  }
  //   if (shift != 0) {
  //   uint64_t original_shift = GetShift(value);
  //   uint64_t valid_shift = shift % (kTotalDataBits);
  //   uint8_t first_bit;

  //   for (uint64_t i = 0; i < valid_shift; ++i) {
  //     for (uint64_t j = 34; j > 0; --j) {
  //       if (j == 34) {
  //         first_bit = value.data[j] & 1;
  //       }
  //       value.data[j] = ((value.data[j] % 128) >> 1) + ((value.data[j - 1] % 2) * 64);
  //     }
  //     value.data[0] = (value.data[0] % 128) / 2 + first_bit * 64;
  //   }

  //   int j = 34;
  //   shift = (original_shift - shift) % (1ll << 35);

  //   while (shift > 0) {
  //     value.data[j] += (shift % 2) * 128;
  //     shift /= 2;
  //     --j;
  //   }
  // }
}


uint239_t FromInt(uint32_t value, uint32_t shift) {
  uint239_t number{};
  int i = 34;

  while (value > 0) {
    number.data[i] = value % 128;
    value /= 128;
    --i;
  }

  LeftShift(number, shift);

  return number;
}

uint239_t FromString(const char* str, uint32_t shift) {
  uint239_t res{};
  uint239_t number_10 = FromInt(10, 0);

  while (*str != '\0') {
    res = res * number_10;
    res = res + FromInt(*str - '0', 0);
    ++str;
  }

  for (int i = 34; i > 0; --i) {
    if (res.data[i] >= 128) {
      res.data[i - 1] += res.data[i] / 128;
      res.data[i] = res.data[i] % 128;
    }
  }

  LeftShift(res, shift);

  return res;
}

uint239_t operator+(const uint239_t& lhs, const uint239_t& rhs) {
  uint239_t lhs_0 = lhs;
  uint64_t shift_lhs_0 = (GetShift(lhs_0)) % (1ll << 35);
  RightShift(lhs_0, shift_lhs_0);
  uint239_t rhs_0 = rhs;
  uint64_t shift_rhs_0 = (GetShift(rhs_0)) % (1ll << 35);
  RightShift(rhs_0, shift_rhs_0);

  uint239_t res{};

  for (int i = 34; i > -1; --i) {
    res.data[i] += (lhs_0.data[i] + rhs_0.data[i]) % 128;
    if (i > 0) {
      res.data[i - 1] = (lhs_0.data[i] + rhs_0.data[i]) / 128;
    }
  }

  for (int i = 34; i > 0; --i) {
    if (res.data[i] >= 128) {
      if (i == 0) {
        std::cout << "Ошибка, число не должно превышать 2^239-1" << std::endl;
        exit(1);
      }
      res.data[i - 1] += res.data[i] / 128;
      res.data[i] = res.data[i] % 128;
    }
  }


  uint64_t res_shift = (shift_lhs_0 + shift_rhs_0) % (1ll << 35);
  LeftShift(res, res_shift);

  return res;
}

uint239_t operator-(const uint239_t& lhs, const uint239_t& rhs) {
  uint239_t lhs_0 = lhs;
  uint64_t shift_lhs_0 = (GetShift(lhs_0)) % (1ll << 35);
  RightShift(lhs_0, shift_lhs_0);
  uint239_t rhs_0 = rhs;
  uint64_t shift_rhs_0 = (GetShift(rhs_0)) % (1ll << 35);
  RightShift(rhs_0, shift_rhs_0);

  uint239_t res{};

  for (int i = 34; i > -1; --i) {
    if (lhs_0.data[i] >= rhs_0.data[i]) {
      res.data[i] += lhs_0.data[i] - rhs_0.data[i];
    } else {
      res.data[i] += lhs_0.data[i] + 128 - rhs_0.data[i];
      res.data[i - 1] -= 1;
    }
  }

  uint64_t res_shift = (shift_lhs_0 - shift_rhs_0) % (1ll << 35);
  LeftShift(res, res_shift);

  return res;
}

uint239_t operator*(const uint239_t& lhs, const uint239_t& rhs) {
  uint239_t lhs_0 = lhs;
  uint64_t shift_lhs_0 = (GetShift(lhs_0)) % (1ll << 35);
  RightShift(lhs_0, shift_lhs_0);
  uint239_t rhs_0 = rhs;
  uint64_t shift_rhs_0 = (GetShift(rhs_0)) % (1ll << 35);
  RightShift(rhs_0, shift_rhs_0);

  uint239_t res{};

  for (int i = 34; i >= 0; i--) {
    for (int j = 34; j >= 0; j--) {
      res.data[i] += (lhs_0.data[i] * rhs_0.data[j]) % 128;
      if (i > 0) {
        res.data[i - 1] += (lhs_0.data[i] * rhs_0.data[j]) / 128;
      }
      int k = i;
      while (res.data[k] >= 128) {
        if (k == 0) {
          std::cout << "Ошибка, число не должно превышать 2^239-1" << std::endl;
          exit(1);
        }
        res.data[k - 1] += res.data[k] / 128;
        res.data[k] %= 128;
        k--;
      }
    }
  }

  LeftShift(res, shift_lhs_0 + shift_rhs_0);

  return res;
}

uint239_t operator/(const uint239_t& lhs, const uint239_t& rhs) {
  uint239_t lhs_0 = lhs;
  uint64_t shift_lhs_0 = (GetShift(lhs_0)) % (1ll << 35);
  
  RightShift(lhs_0, shift_lhs_0);
  uint239_t rhs_0 = rhs;
  uint64_t shift_rhs_0 = (GetShift(rhs_0)) % (1ll << 35);
  RightShift(rhs_0, shift_rhs_0);

  uint239_t res{};

  if (rhs_0 == FromInt(0, 0)) {
    std::cout << "Ошибка, на ноль делить нельзя!!" << std::endl;
    exit(1);
  }

  while (lhs_0 >= rhs_0) {
    uint239_t current_res = FromInt(1, 0);
    uint239_t rhs_1 = rhs_0;
    while (lhs_0 >= (rhs_1 * FromInt(2, 0))) {
      rhs_1 = rhs_1 * FromInt(2, 0);
      current_res = current_res * FromInt(2, 0);
    }
    res = res + current_res;
    lhs_0 = lhs_0 - rhs_1;
  }

  for (int i = 34; i > 0; --i) {
    if (res.data[i] >= 128) {
      res.data[i - 1] += res.data[i] / 128;
      res.data[i] = res.data[i] % 128;
    }
  }

  uint64_t res_shift = (shift_lhs_0 - shift_rhs_0) % (1ll << 35);
  LeftShift(res, res_shift);

  return res;
}

bool operator==(const uint239_t& lhs, const uint239_t& rhs) {
  uint239_t lhs_0 = lhs;
  uint64_t shift_lhs_0 = (GetShift(lhs_0)) % (1ll << 35);
  RightShift(lhs_0, shift_lhs_0);
  uint239_t rhs_0 = rhs;
  uint64_t shift_rhs_0 = (GetShift(rhs_0)) % (1ll << 35);
  RightShift(rhs_0, shift_rhs_0);

  bool flag = true;

  for (int i = 0; i < 35; ++i) {
    if (lhs_0.data[i] % 128 != rhs_0.data[i] % 128) {
      flag = false;
      break;
    }
  }

  return flag;
}

bool operator!=(const uint239_t& lhs, const uint239_t& rhs) {
  uint239_t lhs_0 = lhs;
  uint64_t shift_lhs_0 = (GetShift(lhs_0)) % (1ll << 35);
  RightShift(lhs_0, shift_lhs_0);
  uint239_t rhs_0 = rhs;
  uint64_t shift_rhs_0 = (GetShift(rhs_0)) % (1ll << 35);
  RightShift(rhs_0, shift_rhs_0);

  bool flag = false;

  for (int i = 0; i < 35; ++i) {
    if (lhs_0.data[i] % 128 != rhs_0.data[i] % 128) {
      flag = true;
      break;
    }
  }

  return flag;
}

bool operator>=(const uint239_t& lhs, const uint239_t& rhs) {
  uint239_t lhs_0 = lhs;
  uint64_t shift_lhs_0 = (GetShift(lhs_0)) % (1ll << 35);
  RightShift(lhs_0, shift_lhs_0);
  uint239_t rhs_0 = rhs;
  uint64_t shift_rhs_0 = (GetShift(rhs_0)) % (1ll << 35);
  RightShift(rhs_0, shift_rhs_0);

  bool flag = true;

  for (int i = 0; i < 35; ++i) {
    if (lhs_0.data[i] % 128 > rhs_0.data[i] % 128) {
      break;
    }
    if (lhs_0.data[i] % 128 < rhs_0.data[i] % 128) {
      flag = false;
      break;
    }
  }

  return flag;
}

std::ostream& operator<<(std::ostream& stream, const uint239_t& value) {
  uint64_t shift_value = GetShift(value);
  uint239_t value_0 = value;
  RightShift(value_0, (shift_value % (1ll << 35)));

  uint239_t value_1 = value;
  uint64_t count = 0;
  uint239_t number_10 = FromInt(10, 0);
  uint239_t number_0 = FromInt(0, 0);

  while ((value_1 >= number_0) && (value_1 != number_0)) {
    value_1 = value_1 / number_10;
    ++count;
  }

  if (count == 0) {
    count = 1;
  }

  uint8_t res[count];

  for (int i = 0; i < count; ++i) {
    uint239_t value_2 =  value_0 / number_10;
    res[i] = (value_0 - (value_2) * number_10).data[34];
    value_0 = value_2;
  }

  while (count > 0) {
    stream << (+res[count - 1]);
    --count;
  }
  stream << " (сдвиг = " << shift_value << ")";

  return stream;
}

uint64_t GetShift(const uint239_t& value) {
  uint64_t shift = value.data[0] / 128;

  for (int i = 1; i < 35; ++i) {
    shift *= 2;
    shift += (value.data[i] / 128);
  }

  return shift;
}
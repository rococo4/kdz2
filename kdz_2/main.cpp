#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
std::vector<std::vector<long long>> get_times(std::string &s, int insert = 0);
void naive_search(std::string &find, std::string &string) {
  for (auto i = 0; i < string.size(); i++) {
    bool flag = false;
    for (auto j = 0; j < find.size(); j++) {
      if (string[i + j] != find[j] && find[j] != '?') {
        flag = true;
        break;
      }
    }
    if (!flag) {
      std::cout << "find";
    }
  }
}
std::vector<int> compute_prefix_function(std::string pattern) {
  int n = int(pattern.size());
  std::vector<int> prefix(n);

  prefix[0] = 0;
  for (int i = 1; i < n; i++) {
    int j = prefix[i - 1];
    while (j > 0 && pattern[i] != pattern[j]) {
      j = prefix[j - 1];
    }
    if (pattern[i] == pattern[j]) {
      j++;
    }
    prefix[i] = j;
  }

  return prefix;
}

std::set<char> get_alphabet(std::string &find) {
  std::set<char> output;
  for (auto i : find) {
    output.insert(i);
  }
  return output;
}
void change_string(const std::string &string, const std::set<char> &alphabet,
                   std::vector<std::string> &all_possible_strings) {
  bool flag = false;
  for (auto i = 0; i < string.size(); i++) {
    if (string[i] == '?') {
      for (auto j : alphabet) {
        std::string copy = string;
        copy[i] = j;
        flag = true;
        change_string(copy, alphabet, all_possible_strings);
      }
    }
  }
  if (!flag) {
    all_possible_strings.push_back(string);
  }
}
std::vector<std::string> get_all_possible_strings(const std::string &find,
                                                  std::string &string) {
  std::vector<std::string> all_possible_strings;
  auto alphabet = get_alphabet(string);
  change_string(find, alphabet, all_possible_strings);
  return all_possible_strings;
}
void kmp_search(std::string &find, std::string &string) {
  int m = int(find.size());
  auto possible_strings = get_all_possible_strings(find, string);
  for (auto s : possible_strings) {
    std::vector<int> prefix = compute_prefix_function(s);
    int j = 0;
    for (char i : string) {
      while (j > 0 && i != s[j]) {
        j = prefix[j - 1];
      }
      if (i == s[j]) {
        j++;
      }
      if (j == m) {
        continue;
      }
    }
  }
}

void kmp_search_refined_faces(std::string find_with_questions,
                              std::string string) {
  size_t m = find_with_questions.size();
  size_t n = string.size();
  auto strings = get_all_possible_strings(find_with_questions, string);
  for (auto find : strings) {
    size_t k = 0;
    std::vector<int> pi(m, 0);
    std::vector<int> brs(m, 0);
    for (size_t i = 1; i < m; ++i) {
      while (k > 0 && find[k] != find[i]) {
        k = pi[k - 1];
      }
      if (find[k] == find[i]) {
        k += 1;
      }
      pi[i] = int(k);
    }

    for (size_t i = 1; i < m; ++i) {
      if (find[pi[i - 1]] != find[i]) {
        brs[i - 1] = pi[i - 1];
      } else {
        brs[i - 1] = brs[pi[i - 1]];
      }
    }

    size_t kol = 0;
    for (size_t i = 0; i < n; ++i) {
      while (kol > 0 && find[kol] != string[i]) {
        kol = brs[kol - 1];
      }

      if (find[kol] == string[i]) {
        kol += 1;
      }
      if (kol == m) {
        kol = pi[kol - 1];
      }
    }
  }
}
std::string generate_binary(int count) {
  std::string str(count, ' ');
  srand(time(nullptr));

  for (int i = 0; i < count; i++) {
    int r = rand() % 2;
    if (r == 0) {
      str[i] = 'a';
    } else {
      str[i] = 'b';
    }
  }
  return str;
}
std::string generate_four_letters(int count) {
  std::string str(count, ' ');
  srand(time(nullptr));

  for (int i = 0; i < count; i++) {
    int r = rand() % 4;
    if (r == 0) {
      str[i] = 'a';
    } else if (r == 1) {
      str[i] = 'b';
    } else if (r == 2) {
      str[i] = 'c';
    } else if (r == 3) {
      str[i] = 'd';
    }
  }
  return str;
}
void insert_symbols(std::string &s, int count) {
  srand(time(nullptr));
  for (auto i = 0; i < count; i++) {
    int r = rand() % s.size();
    s[r] = '?';
  }
}
std::vector<std::vector<long long>> count_times_for_10000_bin_no_insertions() {

  std::string s = generate_binary(10000);
  std::vector<std::vector<long long int>> times = get_times(s);
  return times;
}
std::vector<std::vector<long long int>> get_times(std::string &s, int insert) {
  int step = 100;
  srand(time(nullptr));
  int pos = rand() % 3000;
  std::vector<std::vector<long long>> times(30, std::vector<long long>(3, 0));
  int i = 0;
  auto start = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::high_resolution_clock::now() - start;
  for (auto j = 0; j < 10; j++) {
    while (step <= 3000) {
      auto sub_str = s.substr(pos, step);
      if (insert > 0) {
        insert_symbols(sub_str, insert);
      }

      start = std::chrono::high_resolution_clock::now();
      naive_search(sub_str, s);
      elapsed = std::chrono::high_resolution_clock::now() - start;
      times[i][0] +=
          std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();

      start = std::chrono::high_resolution_clock::now();
      kmp_search(sub_str, s);
      elapsed = std::chrono::high_resolution_clock::now() - start;
      times[i][1] +=
          std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();

      start = std::chrono::high_resolution_clock::now();
      kmp_search_refined_faces(sub_str, s);
      elapsed = std::chrono::high_resolution_clock::now() - start;
      times[i][2] +=
          std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();

      step += 100;
      i++;
    }
  }
  for (auto &time : times) {
    for (long long &x : time) {
      x /= 10;
    }
  }
  return times;
}
std::vector<std::vector<long long>> count_times_for_100000_bin_no_insertions() {
  std::string s = generate_binary(100000);
  std::vector<std::vector<long long int>> times = get_times(s);
  return times;
}
std::vector<std::vector<long long>> count_times_for_10000_four_no_insertions() {
  std::string s = generate_four_letters(10000);
  std::vector<std::vector<long long int>> times = get_times(s);
  return times;
}
std::vector<std::vector<long long>>
count_times_for_100000_four_no_insertions() {
  std::string s = generate_four_letters(100000);
  std::vector<std::vector<long long int>> times = get_times(s);
  return times;
}
std::vector<std::vector<long long>>
count_times_for_10000_bin_insertions(int count) {

  std::string s = generate_binary(10000);
  std::vector<std::vector<long long int>> times = get_times(s, count);
  return times;
}
std::vector<std::vector<long long>>
count_times_for_100000_bin_insertions(int count) {
  std::string s = generate_binary(100000);
  std::vector<std::vector<long long int>> times = get_times(s, count);
  return times;
}
std::vector<std::vector<long long>>
count_times_for_10000_four_insertions(int count) {
  std::string s = generate_four_letters(10000);
  std::vector<std::vector<long long int>> times = get_times(s, count);
  return times;
}
std::vector<std::vector<long long>>
count_times_for_100000_four_insertions(int count) {
  std::string s = generate_four_letters(100000);
  std::vector<std::vector<long long int>> times = get_times(s, count);
  return times;
}
void write_in_file(const std::string &name,
                   const std::vector<std::vector<long long>> &time) {
  std::ofstream file;
  file.open(name);
  for (auto i : time) {
    for (auto j = 0; j < 3; j++) {
      if (j != 2) {
        file << i[j] << ";";
      } else {
        file << i[j] << ";"
             << "\n";
      }
    }
  }
  file.close();
}
int main() {
  write_in_file("count_times_for_10000_bin_no_insertions.csv",
                count_times_for_10000_bin_no_insertions());
  write_in_file("count_times_for_10000_four_no_insertions.csv",
                count_times_for_10000_four_no_insertions());
  write_in_file("count_times_for_100000_bin_no_insertions.csv",
                count_times_for_100000_bin_no_insertions());
  write_in_file("count_times_for_100000_four_no_insertions.csv",
                count_times_for_100000_four_no_insertions());
  for (auto i = 1; i < 5; i++) {
    write_in_file("count_times_for_10000_bin_" + std::to_string(i) +
                      "_insertions"
                      ".csv",
                  count_times_for_10000_bin_insertions(i));
    write_in_file("count_times_for_10000_four_" + std::to_string(i) +
                      "_insertions"
                      ".csv",
                  count_times_for_10000_four_insertions(i));
    write_in_file("count_times_for_100000_bin_" + std::to_string(i) +
                      "_insertions"
                      ".csv",
                  count_times_for_100000_bin_insertions(i));
    write_in_file("count_times_for_100000_four_" + std::to_string(i) +
                      "_insertions"
                      ".csv",
                  count_times_for_100000_four_insertions(i));
  }
}
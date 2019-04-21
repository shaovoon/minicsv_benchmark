# [SSO effects] MiniCSV wins string_view CSV Parser in benchmark

## CSV Libraries used

The 1st CSV parser is written by Vincent La while the other 2 parsers (which also include writer) are written by me.

* [string_view CSV Parser](https://github.com/vincentlaucsb/csv-parser) by Vincent La
* [MiniCSV](https://github.com/shaovoon/minicsv) is implemented with STL File streams
* [CSVStream](https://github.com/shaovoon/csv_stream) is a reimplementation of MiniCSV with C File API to avoid code bloat in asm.js

## Benchmark code

There is a CSV Generator project in the repo.

```Cpp
timer stopwatch;
std::string csv_file = "d:\\example.csv";
string_view sv;
stopwatch.start("csv_parser");
for (int a = 0; a < MAX_LOOP; ++a)
{
    CSVReader reader(csv_file.c_str());
    for (CSVRow& row : reader) { // Input iterator
        for (CSVField& field : row) {
            // For efficiency, get<>() produces a string_view
            sv = field.get<>();
        }
    }
    // printing sv resulting in crash!
    //std::cout << sv << std::endl;
}
stopwatch.stop();

std::string str;
stopwatch.start("MiniCSV");
for (int a = 0; a < MAX_LOOP; ++a)
{
    mini::csv::ifstream is(csv_file.c_str());
    is.enable_trim_quote_on_str(false, '\"');
    is.set_delimiter(',', "$$");
    if (is.is_open())
    {
        while (is.read_line())
        {
            size_t total = is.num_of_delimiter() + 1;
            for (size_t i = 0; i < total; ++i)
                is >> str;
        }
        //std::cout << str << std::endl;
    }
}
stopwatch.stop();

str = "";
stopwatch.start("CSV Stream");
for (int a = 0; a < MAX_LOOP; ++a)
{
    capi::csv::ifstream is(csv_file.c_str());
    is.enable_trim_quote_on_str(false, '\"');
    is.set_delimiter(',', "$$");
    if (is.is_open())
    {
        while (is.read_line())
        {
            size_t total = is.num_of_delimiter() + 1;
            for (size_t i = 0; i < total; ++i)
                is >> str;
        }
        //std::cout << str << std::endl;
    }
}
stopwatch.stop();

str = "";
stopwatch.start("CSV Cached Stream");
for (int a = 0; a < MAX_LOOP; ++a)
{
    capi::csv::icachedfstream is(csv_file.c_str());
    is.enable_trim_quote_on_str(false, '\"');
    is.set_delimiter(',', "$$");
    while (is.read_line())
    {
        size_t total = is.num_of_delimiter() + 1;
        for (size_t i = 0; i < total; ++i)
            is >> str;
    }
    //std::cout << str << std::endl;
}
stopwatch.stop();
```

I only benchmark in VC++ 2017.

## Benchmark of every column is 12 chars in length

The file is read 10 times.

The length is within Short String Buffer(SSO) limit to avoid heap allocation

```
csv_parser timing:1085ms
MiniCSV timing:719ms
CSV Stream timing:1864ms
CSV Cached Stream timing:528ms
```

## Benchmark of every column is 30 chars in length

The file is read 10 times.

The length is outside SSO limit, memory has to allocated on the heap! Now string_view csv_parser wins.

```
csv_parser timing:1488ms
MiniCSV timing:1780ms
CSV Stream timing:4962ms
CSV Cached Stream timing:1054ms
```

Note: CSV Cached Stream read the whole file in memory at once.
Note: Benchmark could be different with other C++ compiler like G++ and Clang++ which I do not have access now.
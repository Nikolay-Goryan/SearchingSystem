## SearchingSystem

This project implements a simple searching system. The programm gets the documents (each document is one line in input file) and the list of requests (words and phrases that have to be found in given documents). The programm returns the list of maximum 5 documents for each request, in which the words from request were found.

### Example

**For documets:**\
```
"london is the capital of great britain\n
    paris is the capital of france\n
    berlin is the capital of germany\n
    rome is the capital of italy\n
    madrid is the capital of spain\n
    lisboa is the capital of portugal\n
    bern is the capital of switzerland\n
    moscow is the capital of russia\n
    kiev is the capital of ukraine\n
    minsk is the capital of belarus\n
    astana is the capital of kazakhstan\n
    beijing is the capital of china\n
    tokyo is the capital of japan\n
    bangkok is the capital of thailand\n
    welcome to moscow the capital of russia the third rome\n
    amsterdam is the capital of netherlands\n
    helsinki is the capital of finland\n
    oslo is the capital of norway\n
    stockgolm is the capital of sweden\n
    riga is the capital of latvia\n
    tallin is the capital of estonia\n
    warsaw is the capital of poland\n"
```
**and request:**
```
"moscow is the capital of russia\n"
```
**The result will be:**

```
"moscow is the capital of russia:\n
      {docid: 7, hitcount: 6}\n
      {docid: 14, hitcount: 6}\n
      {docid: 0, hitcount: 4}\n
      {docid: 1, hitcount: 4}\n
      {docid: 2, hitcount: 4}\n"
```

#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <deque>
using namespace std;


struct Entry {
    size_t docID_, hitcount_;
};

class InvertedIndex {
public:
	InvertedIndex() = default;
	void Add(string&& document);
	const vector<Entry>& Lookup(const string_view& word) const;

	const string& GetDocument(size_t id) const {
		return docs[id];
  }
	size_t getDocsSize() const;
	const map<string_view, vector<Entry>> getWordIndex() const {
		return index_;
	}

private:
	map<string_view, vector<Entry>> index_;
	deque<string> docs;
};

class SearchServer {
public:
	SearchServer() = default;
	explicit SearchServer(istream& document_input);
	void UpdateDocumentBase(istream& document_input);
	void AddQueriesStream(istream& query_input, ostream& search_results_output);


	const InvertedIndex getIndex() const {
		return index;
	}
private:
	InvertedIndex index;
};

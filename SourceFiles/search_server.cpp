#include "search_server.h"
#include "iterator_range.h"
#include "profile.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>


//--------------------------------------------------------------------------------------------------


using namespace std;
using namespace chrono;
struct TotalDuration {

	string message;
	steady_clock::duration value;
	
	explicit TotalDuration(const string& msg = "")
	: message(msg + ": "),
	value(0)
	{ }
	
	~TotalDuration() { ostringstream os; os << message
		<< duration_cast<milliseconds>(value).count()
		<< " ms" << endl; cerr << os.str();
	}
};

class AddDuration {
public:
	explicit AddDuration(steady_clock::duration& dest)
	: add_to(dest)
	, start(steady_clock::now())
	{ }
	
	explicit AddDuration(TotalDuration& dest) : AddDuration(dest.value)
	{ }
	~AddDuration() {
		add_to += steady_clock::now() - start;
	}
private:
	steady_clock::duration& add_to;
	steady_clock::time_point start;
};

#define ADD_DURATION(value) \
	AddDuration UNIQ_ID(__LINE__){value};


istream& ReadLine(istream& input, string& s, TotalDuration& dest) {
	ADD_DURATION(dest);
	return getline(input, s);
}


vector<string> SplitIntoWords(const string& line, TotalDuration& dest) {
	istringstream words_input(line);
	ADD_DURATION(dest);
	return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

vector<string_view> SplitIntoWords(string_view line) {
	vector<string_view> words;
	auto pos = line.find_first_not_of(' ');
	line.remove_prefix(pos);
		while(1) {
		pos = line.find(' ');
		words.push_back(line.substr(0, pos));
		line.remove_prefix(pos);
		if(pos == line.npos) {
			break;
		}
		pos = line.find_first_not_of(' ');
		if(pos == line.npos) {
			break;
		}
		line.remove_prefix(pos);
	}
	return words;
}


SearchServer::SearchServer(istream& document_input) {
	UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
	InvertedIndex new_index;

	for (string current_document; getline(document_input, current_document); ) {
		new_index.Add(move(current_document));
	}

	index = move(new_index);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
	vector<size_t> docid_count(index.getDocsSize());
//	TotalDuration split("Total split");
//	TotalDuration indexing("Total indexing");
//	TotalDuration sorting("Total sort");
	for (string current_query; getline(query_input, current_query); ) {

		for (const auto& word : SplitIntoWords(current_query)) {
//			ADD_DURATION(indexing);
			for (const auto& [docid, hitcount] : index.Lookup(word)) {
				docid_count[docid] += hitcount;
			}
		}

		vector<pair<size_t, size_t>> search_results;
		for(int i = 0; i < index.getDocsSize(); i++) {
			if (docid_count[i] != 0) {
				search_results.push_back({i, docid_count[i]});
				docid_count[i] = 0;
			}
		}

		size_t max_result = 5;
		{
//		ADD_DURATION(sorting);
		partial_sort(
					 search_results.begin(),
					 search_results.begin() + min(max_result, search_results.size()),
					 search_results.end(),
			 [](const pair<size_t, size_t>& lhs, const pair<size_t, size_t>& rhs) {
				int64_t lhs_docid = lhs.first;
				auto lhs_hit_count = lhs.second;
				int64_t rhs_docid = rhs.first;
				auto rhs_hit_count = rhs.second;
				return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
				}
			);
		}

		search_results_output << current_query << ':';
		for (auto [docid, hitcount] : Head(search_results, 5)) {
			search_results_output << " {"
			<< "docid: " << docid << ", "
			<< "hitcount: " << hitcount << '}';
		}
		search_results_output << endl;
	}
}


void InvertedIndex::Add(string&& document) {
	docs.push_back(document);					//allright
	const size_t docid = docs.size() - 1;

	map<string_view, size_t> word_doc_stat;

	for (string_view word : SplitIntoWords(docs.back())) {
		word_doc_stat[word]++;
	}

	for (const auto& [word, hitcount]: word_doc_stat) {
		index_[word].push_back({docid, hitcount});
	}

}


const vector<Entry>& InvertedIndex::Lookup(const string_view& word) const {
    static const vector<Entry> empty;

    if(auto it = index_.find(word); it != index_.end()) {
        return it->second;
    }

    return empty;
}

size_t InvertedIndex::getDocsSize() const {
    return docs.size();
}


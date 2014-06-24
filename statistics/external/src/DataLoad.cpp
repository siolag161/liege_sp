
// #include "DataLoad.hpp"
// #include "CSVParser.hpp"
// #include <string>
// #include <boost/lexical_cast.hpp>

// namespace utl = utility;
// namespace clustering
// {

// typedef std::vector< std::vector<double> > Matrix;
// typedef std::string Label;
// typedef int Position;


// std::shared_ptr< Matrix > loadDataTable( const std::string& infile, const char& sep, const char& quote)
// {

// }


// void loadLabelPosition( std::vector<Label> & labels,
//                         std::vector<unsigned>& ids,
//                         std::vector<Position>& positions,
//                         const std::string& infile ) { 
//   std::ifstream labPosFile(infile.c_str());
//   if (!labPosFile) return;
//   std::vector<Label>().swap(labels); //lab2Pos.clear();
//   std::vector<Position>().swap(positions); //.clear();
//   utl::CSVIterator<std::string> labPosLine(labPosFile);// ++labPosLine;
//   for( ; labPosLine != utl::CSVIterator<std::string>(); ++labPosLine ) {    
//     std::string label =  (*labPosLine)[2];
//     int position = boost::lexical_cast<int>( (*labPosLine)[3]);
//     unsigned id = boost::lexical_cast<unsigned>( (*labPosLine)[1]);
//     ids.push_back(id);
//     labels.push_back(label);
//     positions.push_back(position);
//   }

//   std::cout << "load " << labels.size() << " variables.\n";
// }



// } // namespace clusteringends here. clustering

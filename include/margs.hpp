#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <initializer_list>
#include <algorithm>

namespace margs
{
	enum class Action { STORE, STORE_CONST, STORE_FALSE, STORE_TRUE };
	enum class Type { Unknown, Required, Optional };
	typedef struct entry_
	{
		std::string nargs = "1";
		Action act = margs::Action::STORE;
	} entry;

	class margs
	{
		public:	
			margs() {}

			bool parse( int argc, char *argv[] )
			{
				self = std::string( argv[0] );

				p_entry curr = {};

				// send the args into a vector
				int ii = 1;
				for ( ii = 1; ii < ( argc - count( Type::Required ) ); ii++ )
				{
					if ( !( argv[ii][0] == '-' && argv[ii][1] != '\0' ) )
					{
						entries[curr.var].vals.push_back( std::string( argv[ii] ) );
						continue;
					}

					// find the matching entry
					std::string tmp( argv[ii] );
					auto it = std::find_if( entries.begin(), entries.end(),
								[tmp]( const std::pair<std::string,p_entry> &e)
								{
									return std::find(
											e.second.keys.begin(), 
											e.second.keys.end(), 
											tmp
										) != e.second.keys.end();
								}
							);

					if ( it == entries.end() ) 
					{
						std::cerr << "Unknown key: " << curr.var << std::endl;
						print_help();
						return false;
					}
					curr = (*it).second;
				}

				// loop through the required entries and parse
				auto it = std::find_if( entries.begin(), entries.end(),
							[]( const std::pair<std::string,p_entry>& e )
							{
								return e.second.type == Type::Required;
							}
						);

				std::size_t jj = 0;
				while ( it != entries.end() && argc > ( ii + jj ) )
				{
					curr = (*it).second;
					entries[curr.var].vals.push_back( std::string( argv[ii+jj] ) );
					jj++;

					it = std::find_if( ++it, entries.end(),
							[]( const std::pair<std::string,p_entry>& e )
							{
								return e.second.type == Type::Required;
							}
							);
				}
				if ( jj != count( Type::Required ) )
				{
					std::cerr << "Invalid number of required arguments" << std::endl;
				}

				return true;
			}

			margs* add_optional( std::string var, std::initializer_list<std::string> keys, entry e )
			{
				entries[var] = p_entry( {
							.type = Type::Optional,
					       		.var = var,
							.keys = std::vector<std::string>( keys ),
							.e = e
						} );
				return this;
			}

			margs* add_required( std::string var )
			{
				entries[var] = p_entry( {
					        .type = Type::Required,	
						.var = var,
						.keys = std::vector<std::string>()
						} );
				return this;
			}	

			template <typename T>
			bool get( std::string fld, std::unique_ptr<T>& ret )
			{
				// check if the field is in the map
				auto it = std::find_if( entries.begin(), 
							entries.end(),
							[fld]( const std::pair<std::string,p_entry>& e )
							{
								return e.second.var == fld;	
							} 
						);
				if ( it == entries.end() ) return false;
				if ( !(*it).second.vals.size() ) return false;

				std::stringstream ss( (*it).second.vals[0] );
				if ( !(ss >> *ret) ) return false;
				return true;
			}

			std::size_t count( Type type )
			{
				return std::count_if( entries.begin(),
							entries.end(),
							[type]( const std::pair<std::string,p_entry>& e )
							{
								return e.second.type == type;
							}
						);
			}

			void print_help()
			{
				std::cout << "HELP" << std::endl;	
			}

		private:
			typedef struct p_entry_
			{
				Type type = Type::Unknown;
				std::string var;
				std::vector<std::string> keys = {};
				entry e;
				std::vector<std::string> vals = {};
			} p_entry;

			std::string self;
			std::map<std::string,p_entry> entries;
	};
}	

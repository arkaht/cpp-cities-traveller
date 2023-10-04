#include <iostream>
#include <vector>

class City;

struct CityConnection
{
	int cost { 0 };
	City* city { nullptr };
};

class City
{
public:
	City( std::string name ) : name( name ) {}

	void connect_to( City* city, int cost )
	{
		CityConnection connection;
		connection.city = city;
		connection.cost = cost;
		connections.push_back( connection );
	}

	void print_connections()
	{
		std::cout << name << ": ";

		for ( auto itr = connections.begin(); itr != connections.end(); itr++ )
		{
			CityConnection connection = (*itr);
			std::cout << connection.city->name << "(" << connection.cost << ")";

			if ( itr + 1 != connections.end() )
			{
				std::cout << ", ";
			}
		}

		std::cout << std::endl;
	}

private:
	std::string name;
	std::vector<CityConnection> connections;
};

class World
{
public:
	void add_city( City* city ) 
	{ 
		cities.push_back( city ); 

		std::cout << "New City ";
		city->print_connections();
	}

	std::vector<City*> cities;
};

struct WorldTrip
{
	int cost { 0 };
	std::vector<City*> path;
};

class Traveller
{
public:
	Traveller( std::string name ) : name( name ) {}

	WorldTrip find_less_expensive_world_trip( City* origin, World* world )
	{
		WorldTrip trip;
		return trip;
	}

private:
	std::string name;
};

int main()
{
	//  init
	City A( "A" ),
		 B( "B" ),
		 C( "C" ),
		 D( "D" ),
		 E( "E" );

	//  A connections
	A.connect_to( &B, 2 );
	A.connect_to( &C, 2 );

	//  B connections
	B.connect_to( &A, 2 );
	B.connect_to( &C, 3 );
	B.connect_to( &D, 3 );
	B.connect_to( &E, 1 );

	//  C connections
	C.connect_to( &A, 2 );
	C.connect_to( &B, 3 );
	C.connect_to( &E, 3 );
	C.connect_to( &D, 1 );

	//  D connections
	D.connect_to( &B, 3 );
	D.connect_to( &E, 3 );
	D.connect_to( &C, 1 );

	//  E connections
	E.connect_to( &D, 3 );
	E.connect_to( &C, 3 );
	E.connect_to( &B, 1 );

	//  world setup
	std::cout << "World:" << std::endl;

	World world;
	world.add_city( &A );
	world.add_city( &B );
	world.add_city( &C );
	world.add_city( &D );
	world.add_city( &E );

	std::cout << std::endl;

	//  preparing the world trip
	Traveller traveller( "Hannibal Barca" );
	traveller.find_less_expensive_world_trip( &A, &world );

}

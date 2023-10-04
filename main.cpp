#include <iostream>
#include <vector>
#include <unordered_set>

class City;

struct CityRoute
{
	int cost { 0 };
	City* destination { nullptr };
};

class City
{
public:
	City( std::string name ) : name( name ) {}

	void connect_to( City* city, int cost )
	{
		CityRoute route;
		route.destination = city;
		route.cost = cost;
		routes.push_back( route );
	}
	void print_connections()
	{
		std::cout << name << ": ";

		for ( auto itr = routes.begin(); itr != routes.end(); itr++ )
		{
			CityRoute route = (*itr);
			std::cout << route.destination->name << "(" << route.cost << ")";

			if ( itr + 1 != routes.end() )
			{
				std::cout << ", ";
			}
		}

		std::cout << std::endl;
	}

	std::vector<CityRoute> routes;

private:
	std::string name;
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

struct WorldTripContext
{
	WorldTripContext( City* origin, World* world ) 
		: origin( origin )
	{
		for ( City* city : world->cities )
		{
			if ( city == origin ) continue;
			cities.insert( city );
		}
	}

	int cost { 0 };                    //  current cost
	std::unordered_set<City*> cities;  //  locations to visit
	City* origin;                      //  origin city & destination
};

class Traveller
{
public:
	Traveller( std::string name ) : name( name ) {}

	WorldTrip find_less_expensive_world_trip( WorldTripContext ctx )
	{
		WorldTrip trip;

		bool is_finished = ctx.cities.empty();

		for ( auto itr = ctx.origin->routes.begin(); itr != ctx.origin->routes.end(); itr++ )
		{
			CityRoute& route = (*itr);

			//  find our way back to origin
			if ( is_finished )
			{
				if ( route.destination == ctx.origin )
				{
					trip.cost = ctx.cost;
					break;
				}

				continue;
			}

			//  
			WorldTripContext next_ctx = ctx;
			next_ctx.cost += route.cost;
			next_ctx.cities.erase( route.destination );
			find_less_expensive_world_trip( next_ctx );
		}

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
	traveller.find_less_expensive_world_trip( WorldTripContext( &D, &world ) );
}

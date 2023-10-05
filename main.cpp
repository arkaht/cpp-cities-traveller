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
		std::cout << name << " connected to ";

		for ( auto itr = routes.begin(); itr != routes.end(); itr++ )
		{
			CityRoute route = (*itr);
			std::cout << route.destination->name << "(" << route.cost << ")" << ( itr + 1 != routes.end() ? ", " : "" );
		}

		std::cout << std::endl;
	}

	std::string get_name() { return name; }

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
	City* origin { nullptr };
};

struct WorldTripContext
{
	WorldTripContext( City* origin, World* world ) 
		: origin( origin )
	{
		//  add locations
		for ( City* city : world->cities )
		{
			if ( city == origin ) continue;  //  ignore origin
			locations.insert( city );
		}

		//  initialize path
		path.push_back( origin );
	}

	void print()
	{
		std::cout << "World Trip ";
		for ( auto itr = path.begin(); itr != path.end(); itr++ )
		{
			std::cout << (*itr)->get_name() << ( itr + 1 != path.end() ? " - " : "" );
		}
		std::cout << " (cost " << cost << ")" << std::endl;
	}

	int cost { 0 };                       //  current cost
	std::unordered_set<City*> locations;  //  locations still to visit
	std::vector<City*> path;              //  computing path
	City* origin;                         //  origin city & destination
};

class Traveller
{
public:
	Traveller( std::string name ) : name( name ) {}

	WorldTrip find_less_expensive_world_trip( WorldTripContext ctx )
	{
		std::cout << "Finding World Trip from " << ctx.origin->get_name() << std::endl;

		std::vector<WorldTripContext> possibilities, plans;
		possibilities.push_back( ctx );

		//  find all trips possible
		while ( !possibilities.empty() )
		{
			//std::cout << "Possibilities: " << possibilities.size() << " | Plans: " << plans.size() << std::endl;

			//  get current context
			WorldTripContext current_ctx = possibilities[0];
			possibilities.erase( possibilities.begin() );

			City* city = *( current_ctx.path.end() - 1 );

			bool is_finished = current_ctx.locations.empty();
			for ( auto itr = city->routes.begin(); itr != city->routes.end(); itr++ )
			{
				CityRoute& route = (*itr);

				//  find our way back to origin
				if ( is_finished )
				{
					if ( route.destination == ctx.origin )
					{
						current_ctx.cost += route.cost;
						current_ctx.path.push_back( route.destination );
						plans.push_back( current_ctx );

						//std::cout << "- new plan" << std::endl;
						break;
					}

					continue;
				}

				//  prevent going multiple times to the same city
				if ( current_ctx.locations.find( route.destination ) == current_ctx.locations.end() ) continue;

				//  add possibility
				WorldTripContext next_ctx = current_ctx;
				next_ctx.cost += route.cost;
				next_ctx.locations.erase( route.destination );
				next_ctx.path.push_back( route.destination );
				possibilities.push_back( next_ctx );

				//std::cout << "- new possibility" << std::endl;
			}
		}

		std::cout << "Found Plans: " << plans.size() << std::endl;

		//  find the less expensive trip
		WorldTripContext* cheapest_trip { nullptr };
		for ( WorldTripContext& plan : plans )
		{
			if ( cheapest_trip == nullptr || plan.cost < cheapest_trip->cost )
			{
				cheapest_trip = &plan;
			}

			plan.print();
		}

		//  return world trip
		WorldTrip trip;
		if ( cheapest_trip != nullptr )
		{
			trip.cost = cheapest_trip->cost;
			trip.path = cheapest_trip->path;
			trip.origin = cheapest_trip->origin;

			std::cout << "Recommended:" << std::endl;
			cheapest_trip->print();
		}
		else
		{
			std::cout << "Couldn't find a trip! Check that your visiting locations are connected!" << std::endl;
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
	Traveller traveller( "Tom" );
	for ( City* city : world.cities )
	{
		traveller.find_less_expensive_world_trip( WorldTripContext( city, &world ) );
		std::cout << std::endl;
	}
}

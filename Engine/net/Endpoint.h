/// **************************************** QUAKASTER ENGINE **************************************** 
/// net/Endpoint.h 
/// Purpose: Network endpoint that maintains a list of active connectinos, queues packets, and manages
/// packet I/O
/// **************************************************************************************************
#pragma once
#include "Connection.h"
#include <memory>
#include "core.h"
#include <unordered_map>

namespace qk::net {

	struct Endpoint;

	using EndpointCallback_t = void(*)(Endpoint&, Connection*);

	using ConnectionMap_t = std::unordered_map<ClientID, std::unique_ptr<Connection>>;

	
	/*
	 * Network endpoint. Can be either a client or a server.
	 * Stores a list of connections by ClientID (needs to be replaced with a ConnectionID when we get that going)
	*/
	class Endpoint {
		ClientID    m_ID;
		ENetHost*	m_pEndpoint = nullptr;
		bool		m_Initialized = false;
		// not implemented
	public:
		ConnectionMap_t m_Connections;

		/* Callback for when the endpoint receives an event */
		EndpointCallback_t on_receive = nullptr;
		/* Callback for when an endpoint connects to this endpoint*/
		EndpointCallback_t on_connect = nullptr;

		/* Callback for when an endpoint disconnects to this endpoint*/
		EndpointCallback_t on_disconnect = nullptr;

		

		/*
			Binds this endpoint to a port.

			@param address			the address by which other peers may connect to this endpoint.
			@param port				the port by which other peers may connect to this endpoint.
			@param max_recipiente	the max number of endpoints that receive packets from this endpoint
			@retval					true if endpoint is properly initialized, false otherwise
		*/
		bool init(const char* hostname,
			u16 port,
			u32 max_recipients = 32,
			u8 max_channels = 32,
			u32 in_bandwidth = 0,
			u32 out_bandwidth = 0
		);
		// Destroys this host
		void shutdown();

		// Flushes ingoing and outgoing packets for all connections.
		void flush();

		/*
		Binds this endpoint to a port.

		@param hostname			the foreign address to connect to.
		@param port				the foriegn port to connect to
		@param max_channels		the max number of channels by which to communicate with foriegn endpoints.


		@retval					true if the foriegn endpoint successfully allocates
		*/
		bool connect(const char* hostname, u16 port, u8 max_channels = 32);

		/*
		Disconnect from a peer

		@param id				the id of the client to disconnect from

		*/
		void disconnect(ClientID id);


		~Endpoint();


	};


}
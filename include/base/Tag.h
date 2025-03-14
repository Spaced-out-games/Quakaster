#pragma once


/// <summary>
/// Tag interface for entities for use within EnTT. Make sure 
/// </summary>
struct Tag {
	// These are here so that EnTT doesn't optimize tags away. Do with these bits as you wish
	uint8_t buffer;

};

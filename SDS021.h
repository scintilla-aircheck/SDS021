#pragma once

#include <Arduino.h>


class SDS021
{
public:
	enum class EMessage
	{
		Head = 0xAA,
		Tail = 0xAB
	};

	enum class ECommandId
	{
		Command = 0xB4,
		Data = 0xC0,
		Reply = 0xC5
	};

	enum class EAction
	{
		Mode = 0x2,
		Query = 0x4,
		Id = 0x5,
		State = 0x6,
		Version = 0x7,
		Interval = 0x8,
	};

	struct Version
	{
		byte Year;
		byte Month;
		byte Day;
	};

	/// Instantiates a new SDS021 controller with an associated software serial connection
	SDS021();
	~SDS021();

	/// Initializes the software serial connection
	void Begin();

	/// Gets/sets the current sensor ID association
	int ID();
	void ID(int id = 0xFFFF);

	/// Gets/sets a sensor's mode (true is passive/query mode)
	bool PassiveMode();
	void PassiveMode(bool passive);

	/// Gets/sets a sensor's state (i.e. sleep/working)
	bool Awake();
	void Awake(bool awake);

	/// Gets/sets a sensor's active reporting interval
	byte Interval();
	void Interval(byte minutes);

	/// Gets the most recent PM 2.5 dust reading
	float PM2_5();

	/// Gets the most recent PM 10 dust reading
	float PM10();

	/// Queries the device for an updated data frame
	void Query();

	/// Checks serial buffer for updated data
	bool Update();

private:
	// Message properties
	static const int kBaudRate_ = 9600;
	static const byte kOutputLength_ = 19;
	static const byte kInputLength_ = 10;

	// SDS021 data cache
	int ID_;
	float PM2_5_;
	float PM10_;
	bool Mode_;
	bool State_;
	byte Interval_;
	Version Firmware_;

	/// Creates a new message buffer array with common fields set
	byte* MakeMessage(EAction action, bool set = 0, int address = 0xFFFF);

	/// Writes and destroys an existing message buffer array
	void WriteMessage(byte* buffer);

	/// Calculates a checksum byte for a given range of bytes in an array
	byte calcCheckSum(byte* buffer, int start_idx, int stop_idx);

	/// Validates a messages checksum byte
	bool CheckSum(byte* buffer, int start_idx = 2, int stop_idx = 8);

	/// Combines a high and low byte into an integer
	int CrunchBytes(byte high_byte, byte low_byte);
};

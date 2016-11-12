#ifndef GCN_SIMULATION_DATA_GENERATOR
#define GCN_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class GCNAnalyzerSettings;

class GCNSimulationDataGenerator
{
public:
	GCNSimulationDataGenerator();
	~GCNSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, GCNAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	GCNAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //GCN_SIMULATION_DATA_GENERATOR
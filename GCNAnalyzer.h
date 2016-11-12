#ifndef GCN_ANALYZER_H
#define GCN_ANALYZER_H

#include <Analyzer.h>
#include "GCNAnalyzerResults.h"
#include "GCNSimulationDataGenerator.h"

class GCNAnalyzerSettings;
class ANALYZER_EXPORT GCNAnalyzer : public Analyzer
{
public:
	static const U32 BIT_RATE = 1000000;
	enum {
		TYPE_POLL,
		TYPE_IDENT,
		TYPE_INITIAL,
		TYPE_QUERY,
		TYPE_DATA
	};

	GCNAnalyzer();
	virtual ~GCNAnalyzer();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< GCNAnalyzerSettings > mSettings;
	std::auto_ptr< GCNAnalyzerResults > mResults;
	AnalyzerChannelData* mData;

	GCNSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;

private:
	U8 SampleByte();
	void ReadStopBit();

	U32 mSamplesPerBit;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //GCN_ANALYZER_H

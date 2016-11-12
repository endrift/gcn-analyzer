#include "GCNAnalyzer.h"
#include "GCNAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

GCNAnalyzer::GCNAnalyzer()
:	Analyzer(),
	mSettings( new GCNAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

GCNAnalyzer::~GCNAnalyzer()
{
	KillThread();
}

void GCNAnalyzer::WorkerThread()
{
	mResults.reset( new GCNAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );

	mSampleRateHz = GetSampleRate();

	mData = GetAnalyzerChannelData( mSettings->mInputChannel );

	if( mData->GetBitState() == BIT_LOW )
		mData->AdvanceToNextEdge();

	mSamplesPerBit = mSampleRateHz / BIT_RATE;
	U32 samples_to_first_center_of_first_data_bit = U32( 1.5 * double( mSampleRateHz ) / double( BIT_RATE ) );

	for( ; ; )
	{
		// Go to the first falling edge
		mData->AdvanceToNextEdge();
		Frame frame;
		frame.mStartingSampleInclusive = mData->GetSampleNumber();
		U8 byte = SampleByte();
		if (byte == 0x00) {
			frame.mData1 = 0;
			frame.mType = TYPE_POLL;
		}
		if (byte == 0x40) {
			frame.mType = TYPE_QUERY;
			frame.mData1 = byte << 16;
			frame.mData1 |= SampleByte() << 8;
			frame.mData1 |= SampleByte();
			frame.mEndingSampleInclusive = mData->GetSampleNumber();
			ReadStopBit();
			mResults->AddFrame( frame );

			mData->AdvanceToNextEdge();
			frame.mType = TYPE_DATA;
			frame.mStartingSampleInclusive = mData->GetSampleNumber();
			frame.mData1 = SampleByte();
			frame.mData1 <<= 8;
			frame.mData1 |= SampleByte();
			frame.mData1 <<= 8;
			frame.mData1 |= SampleByte();
			frame.mData1 <<= 8;
			frame.mData1 |= SampleByte();
			frame.mData1 <<= 8;
			frame.mData1 |= SampleByte();
			frame.mData1 <<= 8;
			frame.mData1 |= SampleByte();
			frame.mData1 <<= 8;
			frame.mData1 |= SampleByte();
			frame.mData1 <<= 8;
			frame.mData1 |= SampleByte();
		}
		frame.mEndingSampleInclusive = mData->GetSampleNumber();
		ReadStopBit();

		mResults->AddFrame( frame );
		mResults->CommitResults();
		ReportProgress( frame.mEndingSampleInclusive );
	}
}

U8 GCNAnalyzer::SampleByte()
{
	U8 byte;
	for (int i = 0; i < 8; ++i) {
		U64 starting_sample = mData->GetSampleNumber();
		mData->AdvanceToNextEdge();
		U64 rising_sample = mData->GetSampleNumber();
		mData->AdvanceToNextEdge();
		U64 ending_sample = mData->GetSampleNumber();
		int bit = (rising_sample - starting_sample) < (ending_sample - rising_sample);
		byte |= bit << (7 - i);
	}
	return byte;
}

void GCNAnalyzer::ReadStopBit()
{
	mData->AdvanceToNextEdge();
}

bool GCNAnalyzer::NeedsRerun()
{
	return false;
}

U32 GCNAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 GCNAnalyzer::GetMinimumSampleRateHz()
{
	return BIT_RATE * 4;
}

const char* GCNAnalyzer::GetAnalyzerName() const
{
	return "GameCube Controller";
}

const char* GetAnalyzerName()
{
	return "GameCube Controller";
}

Analyzer* CreateAnalyzer()
{
	return new GCNAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}

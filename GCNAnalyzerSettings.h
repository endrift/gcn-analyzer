#ifndef GCN_ANALYZER_SETTINGS
#define GCN_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class GCNAnalyzerSettings : public AnalyzerSettings
{
public:
	GCNAnalyzerSettings();
	virtual ~GCNAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	Channel mInputChannel;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
};

#endif //GCN_ANALYZER_SETTINGS

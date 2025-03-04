// Copyright 2007-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SETTINGS_H_
#define MUMBLE_MUMBLE_SETTINGS_H_

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QRectF>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>

#include "EchoCancelOption.h"
#include "SearchDialog.h"

// Global helper classes to spread variables around across threads
// especially helpful to initialize things like the stored
// preference for audio transmission, since the GUI elements
// will be created long before the AudioInput object, and the
// latter lives in a separate thread and so cannot touch the
// GUI.

struct Shortcut {
	int iIndex;
	QList< QVariant > qlButtons;
	QVariant qvData;
	bool bSuppress;
	bool operator<(const Shortcut &) const;
	bool isServerSpecific() const;
	bool operator==(const Shortcut &) const;
};

struct ShortcutTarget {
	bool bCurrentSelection;
	bool bUsers;
	QStringList qlUsers;
	QList< unsigned int > qlSessions;
	int iChannel;
	QString qsGroup;
	bool bLinks;
	bool bChildren;
	bool bForceCenter;
	ShortcutTarget();
	bool isServerSpecific() const;
	bool operator<(const ShortcutTarget &) const;
	bool operator==(const ShortcutTarget &) const;
};

quint32 qHash(const ShortcutTarget &);
quint32 qHash(const QList< ShortcutTarget > &);

QDataStream &operator<<(QDataStream &, const ShortcutTarget &);
QDataStream &operator>>(QDataStream &, ShortcutTarget &);
Q_DECLARE_METATYPE(ShortcutTarget)

struct PluginSetting {
	QString path;
	bool enabled;
	bool positionalDataEnabled;
	bool allowKeyboardMonitoring;
};
QDataStream &operator>>(QDataStream &arch, PluginSetting &setting);
QDataStream &operator<<(QDataStream &arch, const PluginSetting &setting);
Q_DECLARE_METATYPE(PluginSetting);


struct OverlaySettings {
	enum OverlayPresets { AvatarAndName, LargeSquareAvatar };

	enum OverlayShow { Talking, Active, HomeChannel, LinkedChannels };

	enum OverlaySort { Alphabetical, LastStateChange };

	enum OverlayExclusionMode { LauncherFilterExclusionMode, WhitelistExclusionMode, BlacklistExclusionMode };

	bool bEnable;

	QString qsStyle;

	OverlayShow osShow;
	bool bAlwaysSelf;
	int uiActiveTime; // Time in seconds for a user to stay active after talking
	OverlaySort osSort;

	float fX;
	float fY;

	qreal fZoom;
	unsigned int uiColumns;

	QColor qcUserName[5];
	QFont qfUserName;

	QColor qcChannel;
	QFont qfChannel;

	QColor qcFps;
	QFont qfFps;

	qreal fBoxPad;
	qreal fBoxPenWidth;
	QColor qcBoxPen;
	QColor qcBoxFill;

	bool bUserName;
	bool bChannel;
	bool bMutedDeafened;
	bool bAvatar;
	bool bBox;
	bool bFps;
	bool bTime;

	qreal fUserName;
	qreal fChannel;
	qreal fMutedDeafened;
	qreal fAvatar;
	qreal fUser[5];
	qreal fFps;

	QRectF qrfUserName;
	QRectF qrfChannel;
	QRectF qrfMutedDeafened;
	QRectF qrfAvatar;
	QRectF qrfFps;
	QRectF qrfTime;

	Qt::Alignment qaUserName;
	Qt::Alignment qaChannel;
	Qt::Alignment qaMutedDeafened;
	Qt::Alignment qaAvatar;

	OverlayExclusionMode oemOverlayExcludeMode;
	QStringList qslLaunchers;
	QStringList qslLaunchersExclude;
	QStringList qslWhitelist;
	QStringList qslWhitelistExclude;
	QStringList qslPaths;
	QStringList qslPathsExclude;
	QStringList qslBlacklist;
	QStringList qslBlacklistExclude;

	OverlaySettings();
	void setPreset(const OverlayPresets preset = AvatarAndName);

	void load();
	void load(QSettings *);
	void save();
	void save(QSettings *);
};

struct Settings {
	enum AudioTransmit { Continuous, VAD, PushToTalk };
	enum VADSource { Amplitude, SignalToNoise };
	enum LoopMode { None, Local, Server };
	enum ChannelExpand { NoChannels, ChannelsWithUsers, AllChannels };
	enum ChannelDrag { Ask, DoNothing, Move };
	enum ServerShow { ShowPopulated, ShowReachable, ShowAll };
	enum TalkState { Passive, Talking, Whispering, Shouting, MutedTalking };
	enum IdleAction { Nothing, Deafen, Mute };
	enum NoiseCancel { NoiseCancelOff, NoiseCancelSpeex, NoiseCancelRNN, NoiseCancelBoth };
	typedef QPair< QList< QSslCertificate >, QSslKey > KeyPair;

	AudioTransmit atTransmit;
	quint64 uiDoublePush;
	quint64 pttHold;

	bool bTxAudioCue;
	static const QString cqsDefaultPushClickOn;
	static const QString cqsDefaultPushClickOff;
	QString qsTxAudioCueOn;
	QString qsTxAudioCueOff;

	bool bTxMuteCue;
	static const QString cqsDefaultMuteCue;
	QString qsTxMuteCue;

	bool bTransmitPosition;
	bool bMute, bDeaf;
	bool bTTS;
	bool bUserTop;
	bool bWhisperFriends;
	int iMessageLimitUserThreshold;
	bool bTTSMessageReadBack;
	bool bTTSNoScope;
	bool bTTSNoAuthor;
	int iTTSVolume, iTTSThreshold;
	/// The Text-to-Speech language to use. This setting overrides
	/// the default language for the Text-to-Speech engine, which
	/// is usually inferred from the current locale.
	///
	/// The language is expected to be in BCP47 form.
	///
	/// The setting is currently only supported by the speech-dispatcher
	/// backend.
	QString qsTTSLanguage;
	int iQuality, iMinLoudness, iVoiceHold, iJitterBufferSize;
	bool bAllowLowDelay;
	NoiseCancel noiseCancelMode;
	int iSpeexNoiseCancelStrength;
	quint64 uiAudioInputChannelMask;

	// Idle auto actions
	unsigned int iIdleTime;
	IdleAction iaeIdleAction;
	bool bUndoIdleActionUponActivity;

	VADSource vsVAD;
	float fVADmin, fVADmax;
	int iFramesPerPacket;
	QString qsAudioInput, qsAudioOutput;
	float fVolume;
	float fOtherVolume;
	bool bAttenuateOthersOnTalk;
	bool bAttenuateOthers;
	bool bAttenuateUsersOnPrioritySpeak;
	bool bOnlyAttenuateSameOutput;
	bool bAttenuateLoopbacks;
	int iOutputDelay;

	QString qsALSAInput, qsALSAOutput;
	uint8_t pipeWireInput, pipeWireOutput;
	QString qsPulseAudioInput, qsPulseAudioOutput;
	QString qsJackClientName, qsJackAudioOutput;
	bool bJackStartServer, bJackAutoConnect;
	QString qsOSSInput, qsOSSOutput;
	int iPortAudioInput, iPortAudioOutput;

	bool bASIOEnable;
	QString qsASIOclass;
	QList< QVariant > qlASIOmic;
	QList< QVariant > qlASIOspeaker;

	QString qsCoreAudioInput, qsCoreAudioOutput;

	QString qsWASAPIInput, qsWASAPIOutput;
	/// qsWASAPIRole is configured via 'wasapi/role'.
	/// It is a string explaining Mumble's purpose for opening
	/// the audio device. This can be used to force Windows
	/// to not treat Mumble as a communications program
	/// (the default).
	///
	/// The default is "communications". When this is set,
	/// Windows treats Mumble as a telephony app, including
	/// potential audio ducking.
	///
	/// Other values include:
	///
	///   "console", which should be used for games, system
	///              notification sounds, and voice commands.
	///
	///   "multimedia", which should be used for music, movies,
	///                 narration, and live music recording.
	///
	/// This is practically a direct mapping of the ERole enum
	/// from Windows: https://msdn.microsoft.com/en-us/library/windows/desktop/dd370842
	QString qsWASAPIRole;

	bool bExclusiveInput, bExclusiveOutput;
	EchoCancelOptionID echoOption;
	bool bPositionalAudio;
	bool bPositionalHeadphone;
	float fAudioMinDistance, fAudioMaxDistance, fAudioMaxDistVolume, fAudioBloom;
	/// Contains the settings for each individual plugin. The key in this map is the Hex-represented SHA-1
	/// hash of the plugin's UTF-8 encoded absolute file-path on the hard-drive.
	QHash< QString, PluginSetting > qhPluginSettings;

	OverlaySettings os;

	int iOverlayWinHelperRestartCooldownMsec;
	bool bOverlayWinHelperX86Enable;
	bool bOverlayWinHelperX64Enable;

	int iLCDUserViewMinColWidth;
	int iLCDUserViewSplitterWidth;
	QMap< QString, bool > qmLCDDevices;

	bool bShortcutEnable;
	bool bSuppressMacEventTapWarning;
	bool bEnableEvdev;
	bool bEnableXInput2;
	bool bEnableGKey;
	bool bEnableXboxInput;
	/// Enable use of UIAccess (Windows's UI automation feature). This allows Mumble to receive WM_INPUT messages when
	/// an application with elevated privileges is in foreground.
	bool bEnableUIAccess;
	QList< Shortcut > qlShortcuts;

	enum MessageLog {
		LogNone         = 0x00,
		LogConsole      = 0x01,
		LogTTS          = 0x02,
		LogBalloon      = 0x04,
		LogSoundfile    = 0x08,
		LogHighlight    = 0x10,
		LogMessageLimit = 0x20,
	};

	int iMaxLogBlocks;
	bool bLog24HourClock;
	int iChatMessageMargins;

	static const QPoint UNSPECIFIED_POSITION;
	QPoint qpTalkingUI_Position;
	bool bShowTalkingUI;
	bool bTalkingUI_LocalUserStaysVisible;
	bool bTalkingUI_AbbreviateChannelNames;
	bool bTalkingUI_AbbreviateCurrentChannel;
	bool bTalkingUI_ShowLocalListeners;
	/// relative font size in %
	int iTalkingUI_RelativeFontSize;
	int iTalkingUI_SilentUserLifeTime;
	int iTalkingUI_ChannelHierarchyDepth;
	int iTalkingUI_MaxChannelNameLength;
	int iTalkingUI_PrefixCharCount;
	int iTalkingUI_PostfixCharCount;
	QString qsTalkingUI_AbbreviationReplacement;

	QString qsHierarchyChannelSeparator;

	int manualPlugin_silentUserDisplaytime;

	QMap< int, QString > qmMessageSounds;
	QMap< int, quint32 > qmMessages;

	QString qsLanguage;

	/// Name of the theme to use. @see Themes
	QString themeName;
	/// Name of the style to use from theme. @see Themes
	QString themeStyleName;

	QByteArray qbaMainWindowGeometry, qbaMainWindowState, qbaMinimalViewGeometry, qbaMinimalViewState, qbaSplitterState,
		qbaHeaderState;
	QByteArray qbaConfigGeometry;
	enum WindowLayout { LayoutClassic, LayoutStacked, LayoutHybrid, LayoutCustom };
	WindowLayout wlWindowLayout;
	ChannelExpand ceExpand;
	ChannelDrag ceChannelDrag;
	ChannelDrag ceUserDrag;
	bool bMinimalView;
	bool bHideFrame;
	enum AlwaysOnTopBehaviour { OnTopNever, OnTopAlways, OnTopInMinimal, OnTopInNormal };
	AlwaysOnTopBehaviour aotbAlwaysOnTop;
	bool bAskOnQuit;
	bool bMinimizeOnQuit;
	bool bCloseOnQuit;
	bool bEnableDeveloperMenu;
	bool bLockLayout;
	bool bHideInTray;
	bool bStateInTray;
	bool bUsage;
	bool bShowUserCount;
	bool bShowVolumeAdjustments;
	bool bShowNicknamesOnly;
	bool bChatBarUseSelection;
	bool bFilterHidesEmptyChannels;
	bool bFilterActive;
	QByteArray qbaConnectDialogHeader, qbaConnectDialogGeometry;
	bool bShowContextMenuInMenuBar;

	// Search settings
	bool searchForUsers;
	bool searchForChannels;
	bool searchCaseSensitive;
	bool searchAsRegex;
	bool searchOptionsShown;
	Search::SearchDialog::UserAction searchUserAction;
	Search::SearchDialog::ChannelAction searchChannelAction;
	QPoint searchDialogPosition;

	QString qsUsername;
	QString qsLastServer;
	ServerShow ssFilter;

	QString qsImagePath;

	bool bUpdateCheck;
	bool bPluginCheck;
	bool bPluginAutoUpdate;

	// PTT Button window
	bool bShowPTTButtonWindow;
	QByteArray qbaPTTButtonWindowGeometry;

	// Network settings
	enum ProxyType { NoProxy, HttpProxy, Socks5Proxy };
	bool bTCPCompat;
	bool bReconnect;
	bool bAutoConnect;
	bool bQoS;
	/// Disables the "Public Internet" section in the connect dialog if set.
	bool bDisablePublicList;
	ProxyType ptProxyType;
	QString qsProxyHost, qsProxyUsername, qsProxyPassword;
	unsigned short usProxyPort;

	/// The ping interval in milliseconds. The Mumble client
	/// will regularly send TCP and UDP pings to the remote
	/// server. This setting specifies the time (in milliseconds)
	/// between each ping message.
	int iPingIntervalMsec;

	/// The connection timeout duration in milliseconds.
	/// If a connection is not fully established to the
	/// server within this duration, the client will
	/// forcefully disconnect.
	int iConnectionTimeoutDurationMsec;

	/// bUdpForceTcpAddr forces Mumble to bind its UDP
	/// socket to the same address as its TCP
	/// connection is using.
	bool bUdpForceTcpAddr;

	/// iMaxInFlightTCPPings specifies the maximum
	/// number of ping messages that the client has
	/// sent, but not yet received a response for
	/// from the server. This value is checked when
	/// the client sends its next ping message. If
	/// the maximum is reached, the connection will
	/// be closed.
	/// If this setting is assigned a value of 0 or
	/// a negative number, the TCP ping check is
	/// disabled.
	int iMaxInFlightTCPPings;

	/// The service prefix that the WebFetch class will use
	/// when it constructs its fully-qualified URL. If this
	/// is empty, no prefix is used.
	///
	/// When the WebFetch class receives a HTTP response which
	/// includes the header "Use-Service-Prefix", this setting
	/// is updated to reflect the received service prefix.
	///
	/// For more information, see the documentation for WebFetch::fetch().
	QString qsServicePrefix;

	// Network settings - SSL
	QString qsSslCiphers;

	// Privacy settings
	bool bHideOS;

	int iMaxImageWidth;
	int iMaxImageHeight;
	KeyPair kpCertificate;
	bool bSuppressIdentity;

	bool bShowTransmitModeComboBox;

	// Accessibility
	bool bHighContrast;

	// Recording
	QString qsRecordingPath;
	QString qsRecordingFile;
	enum RecordingMode { RecordingMixdown, RecordingMultichannel };
	RecordingMode rmRecordingMode;
	int iRecordingFormat;

	// Special configuration options not exposed to UI

	/// Codec kill-switch
	bool bDisableCELT;

	/// Removes the add and edit options in the connect dialog if set.
	bool disableConnectDialogEditing;

	/// Asks the user for consent to ping servers in the public server list if not set.
	bool bPingServersDialogViewed;

	// Config updates
	unsigned int uiUpdateCounter;

	/// Path to SQLite-DB
	QString qsDatabaseLocation;

	// Nonsaved
	LoopMode lmLoopMode;
	float dPacketLoss;
	float dMaxPacketDelay;
	/// If true settings in this structure require a client restart to apply fully
	bool requireRestartToApply;

	bool doEcho() const;
	bool doPositionalAudio() const;

	Settings();
	void load();
	void load(QSettings *);
	void save();
};

#endif

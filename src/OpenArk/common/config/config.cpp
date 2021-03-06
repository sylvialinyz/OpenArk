/****************************************************************************
**
** Copyright (C) 2019 BlackINT3
** Contact: https://github.com/BlackINT3/OpenArk
**
** GNU Lesser General Public License Usage (LGPL)
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/
#include "config.h"
#include "../common.h"

void OpenArkConfig::Init()
{
	auto &&cpath = AppConfigDir() + L"\\openark.ini";
	UNONE::FsCreateDirW(UNONE::FsPathToDirW(cpath));
	appconf_ = new QSettings(WStrToQ(cpath), QSettings::IniFormat);
}

int OpenArkConfig::GetLang(ConfOp op, int &lang)
{
	QString section = "/Main/";
	QString key = section + "lang";
	if (op == CONF_GET) {
		lang = GetValue(key, -1).toInt();
		return lang;
	}
	if (op == CONF_SET) {
		SetValue(key, lang);
		return lang;
	}
	return -1;
}

QStringList OpenArkConfig::GetJunkDirs()
{
	QStringList dirs;

	std::vector<std::wstring> junkdirs;
	// Temp
	junkdirs.push_back(UNONE::OsEnvironmentW(L"%Temp%"));
	junkdirs.push_back(UNONE::OsEnvironmentW(L"%windir%\\Temp"));

	// Recent
	auto appdata = UNONE::OsEnvironmentW(L"%AppData%");
	auto localappdata = UNONE::OsEnvironmentW(L"%LocalAppData%");
	junkdirs.push_back(appdata + L"\\Microsoft\\Windows\\Recent");
	junkdirs.push_back(appdata + L"\\Microsoft\\Office\\Recent");

	// Chrome
	junkdirs.push_back(localappdata + L"\\Google\\Chrome\\User Data\\Default\\Cache");
	junkdirs.push_back(localappdata + L"\\Google\\Chrome\\User Data\\Default\\Code Cache");

	// junkdirs.push_back(L"C:\\AppData\\Roaming");

	return WVectorToQList(junkdirs);
}

QString OpenArkConfig::GetConsole(const QString &name)
{
	QString section = "/Console/";
	auto key = section + name;
	if (name == "History.MaxRecords") {
		return GetValue(key, "2000").toString();
	}
	if (name == "History.FilePath") {
		auto &&default_path = AppConfigDir() + L"\\console\\history.txt";
		if (!UNONE::FsIsExistedW(default_path)) {
			UNONE::FsCreateDirW(UNONE::FsPathToDirW(default_path));
			UNONE::FsWriteFileDataW(default_path, "");
		}
		return GetValue(key, WStrToQ(default_path)).toString();
	}
	return "";
}

void OpenArkConfig::GetMainGeometry(int &x, int &y, int &w, int &h)
{
	QString section = "/Main/";
	auto key = section + "x";
	
	if (!(Contains(section + "x") || Contains(section + "y") ||
		Contains(section + "w") || Contains(section + "h"))) {
		QRect desk = QApplication::desktop()->availableGeometry();
		double scale = (double)desk.height() / desk.width();
		double width = desk.width() / 1.7;
		double height = width * scale;
		double pos_x = desk.width() / 8;
		double pos_y = desk.height() / 8;
		x = (int)pos_x;
		y = (int)pos_y;
		w = (int)width;
		h = (int)height;
		return;
	}

	x = GetValue(section + "x").toInt();
	y = GetValue(section + "y").toInt();
	w = GetValue(section + "w").toInt();
	h = GetValue(section + "h").toInt();
}

void OpenArkConfig::SetMainGeometry(int x, int y, int w, int h)
{
	QString section = "/Main/";
	if (x != -1) SetValue(section + "x", x);
	if (y != -1) SetValue(section + "y", y);
	if (w != -1) SetValue(section + "w", w);
	if (h != -1) SetValue(section + "h", h);
	Sync();
}

void OpenArkConfig::GetPrefMainTab(int &idx)
{
	QString section = "/Preference/";
	idx = GetValue(section + "main_tab", idx).toInt();
}
void OpenArkConfig::SetPrefMainTab(int idx)
{
	QString section = "/Preference/";
	SetValue(section + "main_tab", idx);
	Sync();
}
void OpenArkConfig::GetPrefLevel2Tab(int &idx)
{
	QString section = "/Preference/";
	idx = GetValue(section + "level2_tab", idx).toInt();
}
void OpenArkConfig::SetPrefLevel2Tab(int idx)
{
	QString section = "/Preference/";
	SetValue(section + "level2_tab", idx);
	Sync();
}

OpenArkConfig* OpenArkConfig::confobj_ = nullptr;
OpenArkConfig* OpenArkConfig::Instance()
{
	if (confobj_) return confobj_;
	confobj_ = new OpenArkConfig;
	return confobj_;
}

OpenArkConfig::OpenArkConfig() {

}

OpenArkConfig::~OpenArkConfig() {

}
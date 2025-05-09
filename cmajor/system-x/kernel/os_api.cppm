// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.os.api;

import std.core;
import util;

export namespace cmajor::systemx::kernel {

const int keyBackspace = '\b';
const int keyTab = '\t';
const int keyNewLine = '\n';

const int keyControlA = 0x001;
const int keyControlB = 0x002;
const int keyControlC = 0x003;
const int keyControlD = 0x004;
const int keyControlE = 0x005;
const int keyControlF = 0x006;
const int keyControlG = 0x007;
const int keyControlH = 0x008;
const int keyControlI = 0x009;
const int keyControlJ = 0x00A;
const int keyControlK = 0x00B;
const int keyControlL = 0x00C;
const int keyControlM = 0x00D;
const int keyControlN = 0x00E;
const int keyControlO = 0x00F;
const int keyControlP = 0x010;
const int keyControlQ = 0x011;
const int keyControlR = 0x012;
const int keyControlS = 0x013;
const int keyControlT = 0x014;
const int keyControlU = 0x015;
const int keyControlV = 0x016;
const int keyControlW = 0x017;
const int keyControlX = 0x018;
const int keyControlY = 0x019;
const int keyControlZ = 0x01A;
const int keyEscape = 0x01B;
const int keyFS = 0x01C;
const int keyGS = 0x01D;
const int keyRS = 0x01E;
const int keyUS = 0x01F;

// special keys are mapped to Unicode Private Use Area

const int specialKeyStart = 0xE000;

const int keyDown = specialKeyStart + 0;
const int keyUp = specialKeyStart + 1;
const int keyLeft = specialKeyStart + 2;
const int keyRight = specialKeyStart + 3;
const int keyHome = specialKeyStart + 4;
const int keyMsg = specialKeyStart + 5;
const int keyF1 = specialKeyStart + 6;
const int keyF2 = specialKeyStart + 7;
const int keyF3 = specialKeyStart + 8;
const int keyF4 = specialKeyStart + 9;
const int keyF5 = specialKeyStart + 10;
const int keyF6 = specialKeyStart + 11;
const int keyF7 = specialKeyStart + 12;
const int keyF8 = specialKeyStart + 13;
const int keyF9 = specialKeyStart + 14;
const int keyF10 = specialKeyStart + 15;
const int keyF11 = specialKeyStart + 16;
const int keyF12 = specialKeyStart + 17;
const int keyDel = specialKeyStart + 18;
const int keyIns = specialKeyStart + 19;
const int keyPgDown = specialKeyStart + 20;
const int keyPgUp = specialKeyStart + 21;
const int keyPrint = specialKeyStart + 22;
const int keyEnd = specialKeyStart + 23;
const int keyShiftDel = specialKeyStart + 24;
const int keyShiftEnd = specialKeyStart + 25;
const int keyShiftHome = specialKeyStart + 26;
const int keyShiftLeft = specialKeyStart + 27;
const int keyShiftRight = specialKeyStart + 28;
const int keyResize = specialKeyStart + 29;
const int keyShiftUp = specialKeyStart + 30;
const int keyShiftDown = specialKeyStart + 31;
const int keyControlUp = specialKeyStart + 32;
const int keyControlDown = specialKeyStart + 33;
const int keyControlLeft = specialKeyStart + 34;
const int keyControlRight = specialKeyStart + 35;
const int keyControlPgUp = specialKeyStart + 36;
const int keyControlPgDown = specialKeyStart + 37;
const int keyControlHome = specialKeyStart + 38;
const int keyControlEnd = specialKeyStart + 39;

const int keyControlDel = specialKeyStart + 40;
const int keyControlF1 = specialKeyStart + 41;
const int keyControlF2 = specialKeyStart + 42;
const int keyControlF3 = specialKeyStart + 43;
const int keyControlF4 = specialKeyStart + 44;
const int keyControlF5 = specialKeyStart + 45;
const int keyControlF6 = specialKeyStart + 46;
const int keyControlF7 = specialKeyStart + 47;
const int keyControlF8 = specialKeyStart + 48;
const int keyControlF9 = specialKeyStart + 49;
const int keyControlF10 = specialKeyStart + 50;
const int keyControlF11 = specialKeyStart + 51;
const int keyControlF12 = specialKeyStart + 52;
const int keyShiftPgUp = specialKeyStart + 53;
const int keyShiftPgDown = specialKeyStart + 54;
const int keyShiftF1 = specialKeyStart + 55;
const int keyShiftF2 = specialKeyStart + 56;
const int keyShiftF3 = specialKeyStart + 57;
const int keyShiftF4 = specialKeyStart + 58;
const int keyShiftF5 = specialKeyStart + 59;
const int keyShiftF6 = specialKeyStart + 60;
const int keyShiftF7 = specialKeyStart + 61;
const int keyShiftF8 = specialKeyStart + 62;
const int keyShiftF9 = specialKeyStart + 63;
const int keyShiftF10 = specialKeyStart + 64;
const int keyShiftF11 = specialKeyStart + 65;
const int keyShiftF12 = specialKeyStart + 66;

const int keyAltA = specialKeyStart + 67;
const int keyAltB = specialKeyStart + 68;
const int keyAltC = specialKeyStart + 69;
const int keyAltD = specialKeyStart + 70;
const int keyAltE = specialKeyStart + 71;
const int keyAltF = specialKeyStart + 72;
const int keyAltG = specialKeyStart + 73;
const int keyAltH = specialKeyStart + 74;
const int keyAltI = specialKeyStart + 75;
const int keyAltJ = specialKeyStart + 76;
const int keyAltK = specialKeyStart + 77;
const int keyAltL = specialKeyStart + 78;
const int keyAltM = specialKeyStart + 79;
const int keyAltN = specialKeyStart + 80;
const int keyAltO = specialKeyStart + 81;
const int keyAltP = specialKeyStart + 82;
const int keyAltQ = specialKeyStart + 83;
const int keyAltR = specialKeyStart + 84;
const int keyAltS = specialKeyStart + 85;
const int keyAltT = specialKeyStart + 86;
const int keyAltU = specialKeyStart + 87;
const int keyAltV = specialKeyStart + 88;
const int keyAltW = specialKeyStart + 89;
const int keyAltX = specialKeyStart + 90;
const int keyAltY = specialKeyStart + 91;
const int keyAltZ = specialKeyStart + 92;

const int keyAltF1 = specialKeyStart + 93;
const int keyAltF2 = specialKeyStart + 94;
const int keyAltF3 = specialKeyStart + 95;
const int keyAltF4 = specialKeyStart + 96;
const int keyAltF5 = specialKeyStart + 97;
const int keyAltF6 = specialKeyStart + 98;
const int keyAltF7 = specialKeyStart + 99;
const int keyAltF8 = specialKeyStart + 100;
const int keyAltF9 = specialKeyStart + 101;
const int keyAltF10 = specialKeyStart + 102;
const int keyAltF11 = specialKeyStart + 103;
const int keyAltF12 = specialKeyStart + 104;

const int keyControlShiftLeft = specialKeyStart + 105;
const int keyControlShiftRight = specialKeyStart + 106;
const int keyControlShiftHome = specialKeyStart + 107;
const int keyControlShiftEnd = specialKeyStart + 108;
const int keyControlTab = specialKeyStart + 109;
const int keyShiftTab = specialKeyStart + 110;
const int keyControlIns = specialKeyStart + 111;
const int keyShiftIns = specialKeyStart + 112;

const int specialKeyEnd = specialKeyStart + 112;

std::string OsKeyName(int key);
void* OsCreateHostFile(const char* filePath, bool randomAccess);
void* OsOpenHostFile(const char* filePath, bool randomAccess);
void OsCloseHostFile(void* fileHandle);
void* OsCreateIoCompletionPort();
void* OsAssociateFileWithCompletionPort(void* fileHandle, void* completionPort, uint64_t completionKey);
void OsCloseIoCompletionPort(void* completionPortHandle);
bool OsGetQueuedCompletionStatus(void* completionPortHandle, uint64_t* numberOfBytes, uint64_t* completionKey, void** overlapped);
bool OsPostQueuedCompletionStatus(void* completionPortHandle, uint64_t numberOfBytes, uint64_t completionKey);
void* OsCreateOverlapped(uint64_t offset);
void OsDestroyOverlapped(void* overlapped);
bool OsReadFile(void* fileHandle, void* buffer, uint32_t numberOfBytesToRead, void* overlapped);
bool OsWriteFile(void* fileHandle, void* buffer, uint32_t numberOfBytesToWrite, void* overlapped);
std::string OsGetLogicalDrives();
uint32_t OsGetDriveType(const char* rootPathName);
bool OsFindFirstFile(const std::string& directoryName, std::string& entry, void*& searchHandle);
bool OsFindNextFile(void* searchHandle, std::string& entry);
int OsGetConsoleNumberOfColumns();
int OsGetConsoleNumberOfRows();
std::string OsGetComputerName();
std::string OsGetUserName();
void* OsGetStdHandle(int inOutOrErr);
using CallbackFunc = void();
void OsRegisterConsoleCallBack(void* consoleInputHandle, CallbackFunc* cb, void*& waitHandle);
void OsUnregisterConsoleCallBack(void* consoleInputHandle);
char32_t OsReadConsoleInput(void* consoleInputHandle);
void OsGetConsoleScreenBufferInfo(void* consoleOutputHandle, int32_t* cursorPosX, int32_t* cursorPosY, int32_t* screenSizeX, int32_t* screenSizeY, uint16_t* attrs);
void OsSetConsoleCursorPosition(void* consoleOutputHandle, int cursorPosX, int cursorPosY);
void OsWriteConsole(void* consoleOutputHandle, const char32_t* chars);
void OsSetConsoleTextAttribute(void* consoleOutputHandle, uint16_t attrs);
void OsGetFileTimes(const char* filePath, util::DateTime& accessTime, util::DateTime& modifiedTime, util::DateTime& createTime);
extern const uint32_t fixedDriveType;

} // namespace cmajor::systemx::kernel

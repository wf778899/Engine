#pragma once

#include "Error_Base.h"

enum class RegList_ErrType
{
    ERR_EXISTING_REG_INSERTION,         // ������� ������� � RegionsList ��� ������������� �������� RegionP<> (�.�. ��� ��� ���� ��������� �����)
    ERR_GRAB_FROM_EMPTY_LIST,           // ������ �������, ����� ������ ����
    ERR_CONSISTENT_REG_NOTFOUND,        // ������ ���������� ������ �� ������ � S-List
    ERR_REG_WITH_SUCH_START_NOTFOUND,   // ������ � ����� .start �� ������ � P-List
    ERR_OVERLAPPED_REG_INSERTION,       // ������� ������� ���������������� ������� � P-List
};

class Error_RegionsList : public Error_Base
{
public:
    ERRORS_API Error_RegionsList( RegList_ErrType errType, size_t reg_start, size_t reg_size, const char* file_name, const char* func_name, int line, const char* logFile_name = "ErrorLog.txt" );
    ERRORS_API Error_RegionsList( RegList_ErrType errType, size_t reg_size, const char* file_name, const char* func_name, int line, const char* logFile_name = "ErrorLog.txt" );
    ERRORS_API Error_RegionsList( RegList_ErrType errType, const char* file_name, const char* func_name, int line, const char* logFile_name = "ErrorLog.txt" );

    RegList_ErrType Type() const { return m_errType; }
protected:
    virtual std::string Name() const override;

    virtual void PrintSelf() const override;

private:
    RegList_ErrType m_errType;
    size_t m_reg_start;
    size_t m_reg_size;
};
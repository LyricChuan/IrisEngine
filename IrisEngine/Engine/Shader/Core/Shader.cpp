#include "Shader.h"

LPVOID FShader::GetBufferPointer() const
{
	return ShaderCode->GetBufferPointer();
}

SIZE_T FShader::GetBufferSize() const
{
	return ShaderCode->GetBufferSize();
}

void FShader::BuildShaders(
	const wstring& InFileName,
	const string& InEntryFunName,
	const string& InShadersVersion,
	const D3D_SHADER_MACRO* InShaderMacro)
{
	wchar_t WStringFileName[1024] = { 0 };
	{
		char WToCBuff[1024] = { 0 };
		wchar_t_to_char(WToCBuff,1024,InFileName.c_str());

		char Buff[1024] = { 0 };
		char *FullPath = get_full_path(Buff,1024, WToCBuff);

		Engine_Log("Shader path: [%s]", FullPath);
	
		char_to_wchar_t(WStringFileName,1024,FullPath);
	}

	ComPtr<ID3DBlob> ErrorShaderMsg;
	HRESULT Res = D3DCompileFromFile(WStringFileName,
		InShaderMacro,D3D_COMPILE_STANDARD_FILE_INCLUDE,
		InEntryFunName.c_str(), InShadersVersion.c_str(),
#if _DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION
#else
0
#endif
		,0,&ShaderCode,& ErrorShaderMsg);

	if (ErrorShaderMsg)
	{
		Engine_Log_Error("%s", (char*)ErrorShaderMsg->GetBufferPointer());
	}

	//Ê§°Ü¾Í±ÀÀ£
	ANALYSIS_HRESULT(Res);
	//assert(Res);
}


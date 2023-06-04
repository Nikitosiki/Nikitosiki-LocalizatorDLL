unit Unit1;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ExtCtrls;

type
  PAnsiCharArray = ^TAnsiCharArray;
  TAnsiCharArray = array[0..0] of PAnsiChar;
  TForm1 = class(TForm)
    ComboBox1: TComboBox;
    Label3: TLabel;
    Button1: TButton;
    Panel1: TPanel;
    Label5: TLabel;
    Label1: TLabel;
    procedure Button1Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure ComboBox1Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

//function ReadXML(pathFile: PAnsiChar): Boolean; cdecl; external 'LocalizatorDll.dll' name 'ReadXML' delayed;
//function GetWordd(pathFile: PAnsiChar): PAnsiChar; cdecl; external 'LocalizatorDll.dll' name 'GetWord' delayed;
procedure Start(); cdecl; external 'LocalizatorDll.dll' name 'Start' delayed;
procedure Stop(); cdecl; external 'LocalizatorDll.dll' name 'Stop' delayed;
procedure SetSelectLanguage(nameLang: PAnsiChar); cdecl; external 'LocalizatorDll.dll' name 'SetSelectLanguage' delayed;
function GetWord(key: PAnsiChar): PAnsiChar; cdecl; external 'LocalizatorDll.dll' name 'GetWord' delayed;
function GetSelectLanguage(): PAnsiChar; cdecl; external 'LocalizatorDll.dll' name 'GetSelectLanguage' delayed;
function GetLanguageNames(var size: Integer): PAnsiCharArray; cdecl; external 'LocalizatorDll.dll' name 'GetLanguageNames' delayed;


// ��������� ����������� ����������
procedure RestartApplication;
begin
  WinExec(PAnsiChar(AnsiString(ParamStr(0))), SW_SHOW);
  Application.Terminate;
end;


// ��������� ���������� �����, ��������� � �������
procedure FillTranslationForm();
begin
   Form1.Label5.Caption := (GetWord('LDescriptoin'));
   Form1.Label1.Caption := (GetWord('LDescriptoin2'));
   Form1.Label3.Caption := Utf8ToAnsi(GetWord('LLanguageName'));
   Form1.Button1.Caption := Utf8ToAnsi(GetWord('BClick'));
end;


procedure TForm1.Button1Click(Sender: TObject);
begin
    Label5.Caption := Utf8ToAnsi(GetWord('LDescriptoin3'));
end;


procedure TForm1.FormShow(Sender: TObject);
var
  strings: PAnsiCharArray;
  size: Integer;
  i: Integer;
begin

  // ��������������� ��������
  Start();

  // ��������� ���������� ������ ������� ������
  ComboBox1.Items.Clear;
  strings := GetLanguageNames(size);
  for i := 0 to size - 1 do
     ComboBox1.Items.Add(Utf8ToAnsi(strings[i]));

  // ���������� ��������� ����
  ComboBox1.Text := Utf8ToAnsi(GetSelectLanguage());

  // �������� ���������� (���������) �����
  FillTranslationForm();

end;


procedure TForm1.ComboBox1Change(Sender: TObject);
var
  reboot: Integer;
begin

  // �������� ���� �������
  SetSelectLanguage(PAnsiChar(AnsiToUtf8(ComboBox1.Text)));

  reboot := MessageDlg('������������� ���������, ����� ��������� ���������?', mtConfirmation, [mbYes, mbNo], 0);
  if reboot = mrYes then
    RestartApplication;


end;


procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin

  // �������� ���������� ���������� ������������
  Stop();

end;

end.

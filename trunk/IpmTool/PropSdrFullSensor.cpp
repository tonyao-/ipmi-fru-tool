#include "StdAfx.h"
#include "PropSdrFullSensor.h"

CPropSdrFullSensor::CPropSdrFullSensor(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen)
	: CPropSdr(pGrid, L"Full Sensor Record", m_dwMaxSize, pRecord, nRecordLen)
{
	m_pFullSensorRecord = (FullSensorRecord*)m_pSdrHeader;

	CString strName;
	strName.Format(L"%s Full Sensor Record", CString(m_pFullSensorRecord->m_IdString));
	SetName(strName);
	//RECORD KEY BYTES 
	m_FieldArray.Add(new CField(L"Sensor Owner ID", L"[7:1]-7-bit I2C Slave Address, or 7-bit system software ID\n[0]-0b = ID is IPMB Slave Address, 1b = system software ID"
		, 2
		, new CFieldNum(L"[7:1]-Slave Address", 0, 0, 0x7F, L"%d", L"Identifies the PM described in this descriptor.")
		, new CFieldCombo(L"[0]-ID", L"IPMB Slave Address", L"0=IPMB Slave Address|1=System software ID", L"0b = ID is IPMB Slave Address, 1b = system software ID")
		));
	m_FieldArray.Add(new CField(L"Sensor Owner LUN", L"[7:4] -  Channel Number The Channel Number can be used to specify access to sensors that are located on management controllers that are connected to the BMC via channels other than the primary IPMB. (Note: In IPMI v1.5 the ordering of bits 7:2 of this byte have changed to support the 4-bit channel number.) \n[3:2] -  reserved \n[1:0] -  Sensor Owner LUN. LUN in the Sensor Owner that is used to send/receive IPMB messages to access the sensor.  00b if system software is Sensor Owner."
		, 2
		, new CFieldNum(L"[7:4]-Channel Number ", 0, 0, 0xF, L"%d", L"The Channel Number can be used to specify access to sensors that are located on management controllers that are connected to the BMC via channels other than the primary IPMB. (Note: In IPMI v1.5 the ordering of bits 7:2 of this byte have changed to support the 4-bit channel number.)")
		, new CFieldNum(L"[1:0]-Sensor Owner LUN", 0, 0, 3, L"%d", L"LUN in the Sensor Owner that is used to send/receive IPMB messages to access the sensor.  00b if system software is Sensor Owner. ")
		));
	m_FieldArray.Add(new CFieldNum(L"Sensor Number", 0, 0, 0xFF, L"%d", L"Unique number identifying t he sensor behind a given slave address and LUN. Code FFh reserved."));

	//RECORD BODY BYTES
	m_FieldArray.Add(new CFieldNum(L"Entity ID", 0, 0, 0xFF, L"%d", L"Indicates the physical entity that  the sensor is monitoring or is otherwise associated with the sensor. See Table 43-13, Entity ID Codes ."));
	m_FieldArray.Add(new CField(L"Entity Instance", L""
		, 2
		, new CFieldCombo(L"[7]-ID", L"Physical entity", L"0=Physical entity|1=Logical entity", L"0b = treat entity as a  physical entity per Entity ID table 1b = treat entity as a logical container ent ity. For example, if this bit is set, and the Entity ID is ‘Processor’, the  container entity would be considered to represent a logical ‘Processor Group’  rather than a physical processor. This bit is typically used in conjuncti on with an Entity Association record.")
		, new CFieldNum(L"[6:0]-Instance number", 0, 0, 0x7F, L"%d", L"[6:0]-Instance number for entity. (See section 39.1, System- and Device-relative Entity Instance Values   for more information) 00h-5Fh   system-relative Entity Instance. The Entity Instance number must be unique for each different entity of the same type Entity ID in the system. 60h-7Fh   device-relative Entity Instance. The Entity Instance number must only be unique relative to the management controller providing access to the Entity.")
		));
	m_FieldArray.Add(new CField(L"Sensor Initialization", L""
		, 8
		, new CFieldCombo(L"[7]-Settable Sensor", L"FALSE", L"0=FALSE|1=TRUE", L"1b = Sensor is settable (Support the Set Sensor Reading And Event Status command) \nnote: using this bit to report settable sensors is  optional. I.e. it is ok to report a settable sensor  as ‘not settable’ in the SDR if it is desired to not report this capability to s/w) \n0b = Sensor is not settable [7]  - reserved. Write as 0b.  ")
		, new CFieldCombo(L"[6]-Init Scanning", L"Enable", L"0=Disable|1=Enable", L"1b = enable scanning  (t his bit=1 implies that the sensor accepts the ‘enable/disable scanning’ bit in the  Set Sensor Event Enable command). ")
		, new CFieldCombo(L"[5]-Init Events", L"Enable", L"0=Disable|1=Enable", L"1b = enable events (per Sensor Event Message Control Support bits in Sensor Capabilities field, and per the Event Mask fields, below). ")
		, new CFieldCombo(L"[4]-Init Thresholds", L"FALSE", L"0=FALSE|1=TRUE", L"1b = initialize sens or thresholds (per settable threshold mask below). ")
		, new CFieldCombo(L"[3]-Init Hysteresis", L"FALSE", L"0=FALSE|1=TRUE", L"1b = initialize sens or hysteresis (per Sensor Hysteresis Support bits in the Sensor Capabilities field, below). ")
		, new CFieldCombo(L"[2]-Init Sensor Type", L"FALSE", L"0=FALSE|1=TRUE", L"1b = initialize Sensor Type and Event / Reading Type code ")
		, new CFieldCombo(L"[1]-Event generation", L"Enable",  L"0=Disable|1=Enable", L"[1]-0b = event generation dis abled, 1b = event generation enabled")
		, new CFieldCombo(L"[1]-Sensor scanning", L"Enable",  L"0=Disable|1=Enable", L"[0]-0b = sensor scanning dis abled, 1b = sensor scanning enabled")
		));
	m_FieldArray.Add(new CField(L"Sensor Capabilities", L""
		, 5
		, new CFieldCombo(L"[7]-If Entity is not present or disabled", L"Ignore sensor", L"0=Don't ignore sensor |1=Ignore sensor", L"[7]- 1b = Ignore sensor  if Entity is not present or disabled. 0b = don’t ignore sensor")
		, new CFieldCombo(L"[6]-Sensor Auto Re-arm Support", L"FALSE", L"0=FALSE|1=TRUE", L"Indicates whether the sensor requires m anual rearming, or automatically rearms itself when the event clears. ‘manual’ implies that the  get sensor event status and rearm sensor events commands are supported ")
		, new CFieldCombo(L"[5:4]-Sensor Hysteresis Support ", L"No hysteresis", L"0=No hysteresis|1=Readable|2=Readable and settable|3=Fixed & unreadable", L"1b = enable events (per Sensor Event Message Control Support bits in Sensor Capabilities field, and per the Event Mask fields, below). ")
		, new CFieldCombo(L"[3:2]-Sensor Threshold Access Support ", L"No thresholds", L"0=No thresholds|1=Readable|2=Readable and settable|3=Fixed & unreadable", L"00b = no thresholds.   \n01b =  thresholds are readable, per Reading Mask, below.   \n10b =  thresholds are readable and settable per Reading Mask and Settable Threshold Mask, respectively.   \n11b =  Fixed, unreadable, thresholds.  Which thresholds are supported is reflected by the Reading Mask. T he threshold value fields report the values that are ‘hard-coded’ in the sensor. ")
		, new CFieldCombo(L"[1:0]-Sensor Event Message Control Support ", L"Per control", L"0=Per control|1=Entire sensor only|2=Global disable only|3=No events", L"00b =   no thresholds.   \n01b =  thresholds are readable, per Reading Mask, below.   \n10b =  thresholds are readable and settable per Reading Mask and Settable Threshold Mask, respectively.   \n11b =  Fixed, unreadable, thresholds.  Which thresholds are supported is reflected by the Reading Mask. T he threshold value fields report the values that are ‘hard-coded’ in the sensor. ")
		));

	m_FieldArray.Add(new CFieldCombo(L"Sensor Type", L"Temperature", L"1=Temperature|2=Voltage|3=Current|4=Fan|0x6F=HotSwap", L""));
	m_FieldArray.Add(new CFieldNum(L"Event/Reading Type Code", 0, 0, 0xFF, L"0x%x", L"00h:Event/Reading Type unspecified. \n01h-Threshold.\n02h-0Ch-Generic, Generic Discrete. Indicates a sensor that utilizes an Event/Reading Type code & State bit positions / ev ent offsets from one of the sets specified for Discrete or ‘digit al’ Discrete Event/Reading class in  Table 42-2, Generic Event/Reading Type Codes , below. \
																				   \n6Fh:Sensor-specific Discrete. Indicates that the discrete state information is specific to the sensor type. Stat e bit positions / event offsets for a particular sensor type are specified in the ‘sensor-specific offset’ column in  Table 42-3, Sensor Type Codes, below. \
																				   \n70h-7Fh:OEM Discrete. Indicates that the discrete state information is specific to the OEM identified by the Manufactu rer ID for the IPM device that is providing access to the sensor."
																				   ));
	m_FieldArray.Add(new CField(L"Assertion Event Mask / Lower Threshold Mask", L""
		, 15
		, new CFieldNum(L"[14]-Lower non-recoverable threshold"	, 0, 0, 1, L"%d", L"1b = Lower non-critical threshold is comparison returned")
		, new CFieldNum(L"[13]-Lower critical threshold"		, 0, 0, 1, L"%d",  L"1b = Lower non-critical threshold is comparison returned")
		, new CFieldNum(L"[12]-Lower non-critical threshold"	, 0, 0, 1, L"%d", L"1b = Lower non-critical threshold is comparison returned")

		, new CFieldNum(L"[11]-AGH upper non-recoverable"		, 0, 0, 1, L"%d", L"Assertion event for upper non-recoverable going high supported")
		, new CFieldNum(L"[10]-AGL upper non-recoverable"		, 0, 0, 1, L"%d", L"Assertion event for upper non-recoverable going low supported ")
		, new CFieldNum(L"[9]-AGH upper critical"				, 0, 0, 1, L"%d", L"Assertion event for upper critical going high supported")
		, new CFieldNum(L"[8]-AGL upper critical"				, 0, 0, 1, L"%d", L"Assertion event for upper critical going low supported ")
		, new CFieldNum(L"[7]-AGH upper non-critical"			, 0, 0, 1, L"%d", L"Assertion event for upper non-critical going high supported")
		, new CFieldNum(L"[6]-AGL upper non-critical"			, 0, 0, 1, L"%d", L"Assertion event for upper non-critical going low supported ")

		, new CFieldNum(L"[5]-AGH lower non-recoverable"		, 0, 0, 1, L"%d", L"Assertion event for lower non-recoverable going high supported")
		, new CFieldNum(L"[4]-AGL lower non-recoverable"		, 0, 0, 1, L"%d", L"Assertion event for lower non-recoverable going low supported ")
		, new CFieldNum(L"[3]-AGH lower critical"				, 0, 0, 1, L"%d", L"Assertion event for lower critical going high supported")
		, new CFieldNum(L"[2]-AGL lower critical"				, 0, 0, 1, L"%d", L"Assertion event for lower critical going low supported ")
		, new CFieldNum(L"[1]-AGH lower non-critical"			, 0, 0, 1, L"%d", L"Assertion event for lower non-critical going high supported")
		, new CFieldNum(L"[0]-AGL lower non-critical"			, 0, 0, 1, L"%d", L"Assertion event for lower non-critical going low supported ")
		));

	m_FieldArray.Add(new CField(L"Deassertion Event Mask / Upper Threshold Mask", L""
		, 15
		, new CFieldNum(L"[14]-Lower non-recoverable threshold"	, 0, 0, 1, L"%d", L"1b = Lower non-critical threshold is comparison returned")
		, new CFieldNum(L"[13]-Lower critical threshold"		, 0, 0, 1, L"%d", L"1b = Lower non-critical threshold is comparison returned")
		, new CFieldNum(L"[12]-Lower non-critical threshold"	, 0, 0, 1, L"%d", L"1b = Lower non-critical threshold is comparison returned")

		, new CFieldNum(L"[11]-DGH upper non-recoverable"		, 0, 0, 1, L"%d", L"Deassertion event for upper non-recoverable going high supported")
		, new CFieldNum(L"[10]-DGL upper non-recoverable"		, 0, 0, 1, L"%d", L"Deassertion event for upper non-recoverable going low supported ")
		, new CFieldNum(L"[9]-DGH upper critical"				, 0, 0, 1, L"%d", L"Deassertion event for upper critical going high supported")
		, new CFieldNum(L"[8]-DGL upper critical"				, 0, 0, 1, L"%d", L"Deassertion event for upper critical going low supported ")
		, new CFieldNum(L"[7]-DGH upper non-critical"			, 0, 0, 1, L"%d", L"Deassertion event for upper non-critical going high supported")
		, new CFieldNum(L"[6]-DGL upper non-critical"			, 0, 0, 1, L"%d", L"Deassertion event for upper non-critical going low supported ")

		, new CFieldNum(L"[5]-DGH lower non-recoverable"		, 0, 0, 1, L"%d", L"Deassertion event for lower non-recoverable going high supported")
		, new CFieldNum(L"[4]-DGL lower non-recoverable"		, 0, 0, 1, L"%d", L"Deassertion event for lower non-recoverable going low supported ")
		, new CFieldNum(L"[3]-DGH lower critical"				, 0, 0, 1, L"%d", L"Deassertion event for lower critical going high supported")
		, new CFieldNum(L"[2]-DGL lower critical"				, 0, 0, 1, L"%d", L"Deassertion event for lower critical going low supported ")
		, new CFieldNum(L"[1]-DGH lower non-critical"			, 0, 0, 1, L"%d", L"Deassertion event for lower non-critical going high supported")
		, new CFieldNum(L"[0]-DGL lower non-critical"			, 0, 0, 1, L"%d", L"Deassertion event for lower non-critical going low supported ")
		));

	m_FieldArray.Add(new CField(L"Discrete Reading Mask / Settable Threshold Mask", L"Reading Mask (for non- threshold based sensors)\n\t[15] -  reserved. Write as 0b \n[14:0]-state bits 0 through 14.\n1b = discrete state can be returned by this sensor. \n\n[8-13]Settable Threshold Mask (for  threshold-based sensors)\n[0-5]Readable Threshold Mask (for threshold-based sensors)"
		, 16
			, new CFieldNum(L"[15]"													, 0, 0, 0, L"%d", L"")
			, new CFieldNum(L"[14]"													, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[13]-Settable Mask Upper non-recoverable threshold"	, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[12]-Settable Mask Upper critical threshold"			, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[11]-Settable Mask Upper non-critical threshold"		, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[10]-Settable Mask Lower non-recoverable threshold"	, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[9]-Settable Mask Lower critical threshold"			, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[8]-Settable Mask Lower non-critical threshold"		, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[7]"													, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[6]"													, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[5]-Readable Mask Upper non-recoverable threshold"	, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[4]-Readable Mask Upper critical threshold"			, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[3]-Readable Mask Upper non-critical threshold"		, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[2]-Readable Mask Lower non-recoverable threshold"	, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[1]-Readable Mask Lower critical threshold"			, 0, 0, 1, L"%d", L"")
			, new CFieldNum(L"[0]-Readable Mask Lower non-critical threshold"		, 0, 0, 1, L"%d", L"")
		));

	m_FieldArray.Add(new CField(L"Sensor Units 1", L""
		, 4
		, new CFieldCombo(L"[7:6]-Analog Data Format", L"Unsigned", L"0=Unsigned|1=1's complement (signed)|2=2's complement (signed)|3=Does not return analog (numeric) reading", L"")
		, new CFieldCombo(L"[5:3]-Rate unit", L"None", L"0=None|1=per uS|2=per MS|3=per s|3=per Minute|4=per Hour|5=per Day", L"")
		, new CFieldCombo(L"[2:1]-Modifier unit", L"None", L"0=None|1=Basic Unit / Modifier Unit|2=Basic Unit * Modifier Unit", L"")
		, new CFieldCombo(L"[0]-Percentage", L"Yes", L"0=No|1=Yes", L"")
		));

	m_FieldArray.Add(new CFieldNum(L"Sensor Units 2 - Base Unit", 0, 0, 0xFF, L"0x%x", L"Units Type code: See Table 43-15, Sensor Unit Type Codes"));
	m_FieldArray.Add(new CFieldNum(L"Sensor Units 3 - Modifier Unit", 0, 0, 0xFF, L"0x%x", L"Units Type code, 00h if unused."));
	m_FieldArray.Add(new CFieldNum(L"Linearization ", 0, 0, 0x7F, L"0x%x", L"[7] -  reserved \n[6:0] -  enum (linear, ln, log10, log2, e, ex p10, exp2, 1/x, sqr(x), cube(x), sqrt(x), cube-1 (x) ) - 70h = non-linear. 71h-7Fh = non-linear, OEM defined. "));
	m_FieldArray.Add(new CFieldNum(L"M", 0, 0, 0x3FF, L"0x%x", L"M: LS 10 bits  [2’s complement, signed, 10 bit ‘M’ value.]"));
	m_FieldArray.Add(new CFieldNum(L"Tolerance", 0, 0, 0x1F, L"0x%x", L"Tolerance: 6 bits, unsigned  (Tolerance in +/- ½ raw counts) "));
	m_FieldArray.Add(new CFieldNum(L"B", 0, 0, 0x3FF, L"0x%x", L"M: LS 10 bits  [2’s complement, signed, 10 bit ‘M’ value.]"));
	m_FieldArray.Add(new CFieldNum(L"Accuracy", 0, 0, 0x3FF, L"0x%x", L""));
	m_FieldArray.Add(new CFieldNum(L"AccuracyExp", 0, 0, 0x3, L"0x%x", L"Accuracy exp: 2 bits, unsigned"));
	m_FieldArray.Add(new CFieldCombo(L"Sensor Direction", L"Unspecified", L"0=Unspecified|1=Input|2=Output", L"Sensor Direction. Indicates whether the sensor is monitoring an input or output relative to the given Entity. E.g. if the sensor is monitoring a current, this can be used to specify  whether it is an input voltage or an output voltage."));
	m_FieldArray.Add(new CFieldNum(L"R Exp", 0, 0, 0xF, L"0x%x", L"R (result) exponent 4 bits, 2’s complement, signed "));
	m_FieldArray.Add(new CFieldNum(L"B Exp", 0, 0, 0xF, L"0x%x", L"B exponent 4 bits, 2’s complement, signed"));
	m_FieldArray.Add(new CField(L"Analog characteristic  flags", L""
		, 3
		, new CFieldCombo(L"Normal min specified", L"Yes", L"0=No|1=Yes", L"")
		, new CFieldCombo(L"Normal max specified", L"Yes", L"0=No|1=Yes", L"")
		, new CFieldCombo(L"Normal reading specified", L"Yes", L"0=No|1=Yes", L"")
		));
	m_FieldArray.Add(new CFieldNum(L"Nominal Reading", 0, 0, 0xFF, L"0x%x", L"Given as a raw value. Must be  converted to units-based value using the ‘y=Mx+B’ formula. 1’s or 2’s complement signed or  unsigned per flag bits in Sensor Units 1."));
	m_FieldArray.Add(new CFieldNum(L"Nominal Maximum", 0, 0, 0xFF, L"0x%x", L"Given as a raw value. Must be  converted to units-based value using the ‘y=Mx+B’ formula. 1’s or 2’s complement signed or  unsigned per ‘signed’ bit in Sensor Units 1."));
	m_FieldArray.Add(new CFieldNum(L"Nominal Minimum", 0, 0, 0xFF, L"0x%x", L"Given as a raw value. Must be  converted to units-based value using the ‘y=Mx+B’ formula. Signed or unsigned per ‘si gned’ bit in Sensor Units 1."));
	
	m_FieldArray.Add(new CFieldNum(L"Sensor Maximum Reading", 0, 0, 0xFF, L"0x%x", L"Given as a raw value. Must be converted to units- based value based using the  y=Mx+B formula. Signed or unsigned per ‘signed’ bit in sensor flags. Normally FFh’ for an 8-bit unsigned sensor, but can be a lesser value if the sensor has a restricted range. If max. reading cannot be pre-specified this value should be set to max value, based on data format, (e.g . FFh for an unsigned sensor, 7Fh for 2’s complement, etc.) "));
	m_FieldArray.Add(new CFieldNum(L"Sensor Minimum Reading", 0, 0, 0xFF, L"0x%x", L"Given as a raw value. Must be converted to units-based  value using the ‘y=Mx+B’ formula.  Signed or unsigned per ‘signed’  bit in sensor flags. If min. reading cannot be pre-specified this value s hould be set to min value, based on data format, (e.g. 00h for an unsigned sensor, 80h for 2’s complement, etc.) "));
	
	m_FieldArray.Add(new CFieldNum(L"Upper non-recoverable Threshold", 0, 0, 0xFF, L"0x%x", L"Use of this field is  based on Settable Threshold Ma sk. If the corresponding bit is set in the mask byte and the ‘Init Sensor Thresholds’ bit is also set, then this value will be used for initializing the sensor threshold. Otherwise, this value should be ignored. The thresholds are given as raw values that must be converted to units-based values using the ‘y=Mx+B’ formula."));
	m_FieldArray.Add(new CFieldNum(L"Upper critical Threshold", 0, 0, 0xFF, L"0x%x", L"Use of this field is  based on Settable Threshold Ma sk. If the corresponding bit is set in the mask byte and the ‘Init Sensor Thresholds’ bit is also set, then this value will be used for initializing the sensor threshold. Otherwise, this value should be ignored. The thresholds are given as raw values that must be converted to units-based values using the ‘y=Mx+B’ formula."));
	m_FieldArray.Add(new CFieldNum(L"Upper non-critical Threshold", 0, 0, 0xFF, L"0x%x", L"Use of this field is  based on Settable Threshold Ma sk. If the corresponding bit is set in the mask byte and the ‘Init Sensor Thresholds’ bit is also set, then this value will be used for initializing the sensor threshold. Otherwise, this value should be ignored. The thresholds are given as raw values that must be converted to units-based values using the ‘y=Mx+B’ formula."));
	m_FieldArray.Add(new CFieldNum(L"Lower non-recoverable Threshold", 0, 0, 0xFF, L"0x%x", L"Use of this field is  based on Settable Threshold Ma sk. If the corresponding bit is set in the mask byte and the ‘Init Sensor Thresholds’ bit is also set, then this value will be used for initializing the sensor threshold. Otherwise, this value should be ignored. The thresholds are given as raw values that must be converted to units-based values using the ‘y=Mx+B’ formula."));
	m_FieldArray.Add(new CFieldNum(L"Lower critical Threshold", 0, 0, 0xFF, L"0x%x", L"Use of this field is  based on Settable Threshold Ma sk. If the corresponding bit is set in the mask byte and the ‘Init Sensor Thresholds’ bit is also set, then this value will be used for initializing the sensor threshold. Otherwise, this value should be ignored. The thresholds are given as raw values that must be converted to units-based values using the ‘y=Mx+B’ formula."));
	m_FieldArray.Add(new CFieldNum(L"Lower non-critical Threshold", 0, 0, 0xFF, L"0x%x", L"Use of this field is  based on Settable Threshold Ma sk. If the corresponding bit is set in the mask byte and the ‘Init Sensor Thresholds’ bit is also set, then this value will be used for initializing the sensor threshold. Otherwise, this value should be ignored. The thresholds are given as raw values that must be converted to units-based values using the ‘y=Mx+B’ formula."));
	m_FieldArray.Add(new CFieldNum(L"Positive-going Threshold Hysteresis value ", 0, 0, 0xFF, L"0x%x", L"Positive hysteresis is defined as  the unsigned number of counts that are subtracted from the raw threshold values to create the ‘re-arm’ point for all positive-going thresholds on the sensor. 0 indicates that there is no hysteresis on positive-going thresholds for  this sensor. Hysteresis  values are given as raw counts. That is, to find the degree of hysteresis in units, the value must be converted using the ‘y=Mx+B’ formula. "));
	m_FieldArray.Add(new CFieldNum(L"Negative-going Threshold Hysteresis value", 0, 0, 0xFF, L"0x%x", L"Negative hysteresis is defined as t he unsigned number of counts that are added to the raw threshold value to create the ‘re-arm’ point for all negative-going thresholds on the sensor. 0 indicates t hat there is no hysteresis on negative-going thresholds for this sensor."));

	m_FieldArray.Add(new CFieldNum(L"OEM", 0, 0, 0xFF, L"0x%x", L"Reserved for OEM use. "));

	m_FieldArray.Add(new CField(L"ID String Type/Length", L"Sensor ‘ID’ String Type/Length Code, per Section 43.15, Type/Length Byte Format."
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes",g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));
	m_FieldArray.Add(new CFieldStr(L"ID String Bytes"	, L"1", 0,  16, NULL, NULL, NULL, L"Sensor ID String bytes. Only present if non-zero length in Type/Length field. 16 bytes, maximum. Note: the SDR can  be implemented as a fixed length record. Bytes beyond the ID string bytes  are unspecified and should be ignored."));

	UpdateData(FALSE);
}


CPropSdrFullSensor::~CPropSdrFullSensor(void)
{
}

BOOL CPropSdrFullSensor::UpdateData(BOOL isUpdateData)
{
	int i = 0;
	int nRecordLen = 0;
	if(isUpdateData)
	{
		i = 12;
		m_pFullSensorRecord->m_IdType		= GetByteValue(i	, 0);
		m_pFullSensorRecord->m_IpmbAddress	= GetByteValue(i++	, 1);

		m_pFullSensorRecord->m_ChannelNum		= GetByteValue(i	, 0);
		m_pFullSensorRecord->m_SensorOwnerLun	= GetByteValue(i++	, 1);

		m_pFullSensorRecord->m_SensorNumber		= GetByteValue(i++);
		m_pFullSensorRecord->m_EntityId			= GetByteValue(i++);

		m_pFullSensorRecord->m_islogicalEntity	= GetByteValue(i	, 0);
		m_pFullSensorRecord->m_InstanceNumber	= GetByteValue(i++	, 1);

		m_pFullSensorRecord->m_Settable			= GetByteValue(i	, 0);
		m_pFullSensorRecord->m_InitScanning		= GetByteValue(i	, 1);
		m_pFullSensorRecord->m_InitEvent		= GetByteValue(i	, 2);
		m_pFullSensorRecord->m_InitThresholds	= GetByteValue(i	, 3);
		m_pFullSensorRecord->m_InitHysteresis	= GetByteValue(i	, 4);
		m_pFullSensorRecord->m_InitSensorType	= GetByteValue(i	, 5);
		m_pFullSensorRecord->m_EventEnabled		= GetByteValue(i	, 6);
		m_pFullSensorRecord->m_ScanningEnabled	= GetByteValue(i++	, 7);

		m_pFullSensorRecord->m_IgnoreSensor		= GetByteValue(i	, 0);
		m_pFullSensorRecord->m_AutoRearm		= GetByteValue(i	, 1);
		m_pFullSensorRecord->m_Hysteresis		= GetByteValue(i	, 2);
		m_pFullSensorRecord->m_ThresholdFlag	= GetByteValue(i	, 3);
		m_pFullSensorRecord->m_EventMessage		= GetByteValue(i++	, 4);

		m_pFullSensorRecord->m_SensorType		= GetByteValue(i++);
		m_pFullSensorRecord->m_EventType		= GetByteValue(i++);

		m_pFullSensorRecord->m_LowerThresholdMask = 0;
		for(int j = 14; j>=0; j--)
		{
			m_pFullSensorRecord->m_LowerThresholdMask |= GetByteValue(i, j);
			if(j > 0)
			{
				m_pFullSensorRecord->m_LowerThresholdMask <<= 1;
			}
		}
		i++;

		m_pFullSensorRecord->m_UpperThresholdMask = 0;
		for(int j = 14; j>=0; j--)
		{
			m_pFullSensorRecord->m_UpperThresholdMask |= GetByteValue(i, j);
			if(j > 0)
			{
				m_pFullSensorRecord->m_UpperThresholdMask <<= 1;
			}
		}
		i++;

		m_pFullSensorRecord->m_ReadingMask = 0;
		for(int j = 15; j>=0; j--)
		{
			m_pFullSensorRecord->m_ReadingMask |= GetByteValue(i, j);
			if(j > 0)
			{
				m_pFullSensorRecord->m_ReadingMask <<= 1;
			}
		}
		i++;

		Unit1* pUnit1 = (Unit1*)&m_pFullSensorRecord->m_Uint1;
		pUnit1->m_isSigned		 = GetByteValue(i, 0);
		pUnit1->m_RateUnit		 = GetByteValue(i, 1);
		pUnit1->m_ModifyUnitFlag = GetByteValue(i, 2);
		pUnit1->m_Percentage	 = GetByteValue(i++, 3);

		m_pFullSensorRecord->m_BaseUnit			= GetByteValue(i++);
		m_pFullSensorRecord->m_ModifyUnit		= GetByteValue(i++);
		m_pFullSensorRecord->m_Linearization	= GetByteValue(i++);

		DWORD dw = GetDwValue(i++);
		m_pFullSensorRecord->m_MLs = (uint8)dw;
		m_pFullSensorRecord->m_MMs = (uint8)((dw >> 8) & 0x03);
		m_pFullSensorRecord->m_Tolerance = GetDwValue(i++);

		dw = GetDwValue(i++);
		m_pFullSensorRecord->m_BLs = (uint8)dw;
		m_pFullSensorRecord->m_BMs = (uint8)((dw >> 8) & 0x03);

		dw = GetDwValue(i++);
		m_pFullSensorRecord->m_AccuracyLs =(uint8)dw & 0x3F;
		m_pFullSensorRecord->m_AccuracyMs =(uint8)(dw >> 6) & 0x0F;

		m_pFullSensorRecord->m_AccuracyExp	= GetByteValue(i++);
		m_pFullSensorRecord->m_SensorDirection	= GetByteValue(i++);
		m_pFullSensorRecord->m_Rexp	= GetByteValue(i++);
		m_pFullSensorRecord->m_Bexp	= GetByteValue(i++);

		m_pFullSensorRecord->m_IsMinSpecified		= GetByteValue(i, 0);
		m_pFullSensorRecord->m_IsMaxSpecified		= GetByteValue(i, 1);
		m_pFullSensorRecord->m_IsReadingSpecified	= GetByteValue(i++, 2);

		m_pFullSensorRecord->m_NormalReading	= GetByteValue(i++);
		m_pFullSensorRecord->m_NormalMax		= GetByteValue(i++);
		m_pFullSensorRecord->m_NormalMin		= GetByteValue(i++);
		m_pFullSensorRecord->m_MaxReading		= GetByteValue(i++);
		m_pFullSensorRecord->m_MinReading		= GetByteValue(i++);

		m_pFullSensorRecord->m_UpperNonRecoverable	= GetByteValue(i++);
		m_pFullSensorRecord->m_UpperCiritcal		= GetByteValue(i++);
		m_pFullSensorRecord->m_UpperNonCiritcal		= GetByteValue(i++);
		m_pFullSensorRecord->m_LowerNonRecoverable	= GetByteValue(i++);
		m_pFullSensorRecord->m_LowerCiritcal		= GetByteValue(i++);
		m_pFullSensorRecord->m_LowerNonCiritcal		= GetByteValue(i++);
		m_pFullSensorRecord->m_PoHysteresis			= GetByteValue(i++);
		m_pFullSensorRecord->m_NeHysteresis			= GetByteValue(i++);
		m_pFullSensorRecord->m_Oem					= GetByteValue(i++);

		m_nStrFieldIndex = i;
		m_pFullSensorRecord->m_IdStrType			= GetByteValue(i++, 0);

		m_pFullSensorRecord->m_IdStrLen = GetStrValue(i, m_pFullSensorRecord->m_IdString, sizeof(m_pFullSensorRecord->m_IdString), FALSE);
		SetDwValue(m_nStrFieldIndex, 1, m_pFullSensorRecord->m_IdStrLen);

		nRecordLen = sizeof(FullSensorRecord) 
			- SDR_HEADER_LEN 
			- sizeof(m_pFullSensorRecord->m_IdString) 
			+ m_pFullSensorRecord->m_IdStrLen;


		CPropSdr::UpdateData(nRecordLen, isUpdateData);
	}
	else
	{
		CPropSdr::UpdateData(nRecordLen, isUpdateData);
		i = 12;
		SetDwValue(i,	0, m_pFullSensorRecord->m_IdType);
		SetDwValue(i++, 1, m_pFullSensorRecord->m_IpmbAddress);

		SetDwValue(i,	0, m_pFullSensorRecord->m_ChannelNum);
		SetDwValue(i++, 1, m_pFullSensorRecord->m_SensorOwnerLun);

		SetDwValue(i++, m_pFullSensorRecord->m_SensorNumber);
		SetDwValue(i++, m_pFullSensorRecord->m_EntityId);

		SetDwValue(i,	0, m_pFullSensorRecord->m_islogicalEntity);
		SetDwValue(i++, 1, m_pFullSensorRecord->m_InstanceNumber);

		SetDwValue(i,	0, m_pFullSensorRecord->m_Settable);
		SetDwValue(i,	1, m_pFullSensorRecord->m_InitScanning);
		SetDwValue(i,	2, m_pFullSensorRecord->m_InitEvent);
		SetDwValue(i,	3, m_pFullSensorRecord->m_InitThresholds);
		SetDwValue(i,	4, m_pFullSensorRecord->m_InitHysteresis);
		SetDwValue(i,	5, m_pFullSensorRecord->m_InitSensorType);
		SetDwValue(i,	6, m_pFullSensorRecord->m_EventEnabled);
		SetDwValue(i++,	7, m_pFullSensorRecord->m_ScanningEnabled);

		SetDwValue(i,	0, m_pFullSensorRecord->m_IgnoreSensor);
		SetDwValue(i,	1, m_pFullSensorRecord->m_AutoRearm);
		SetDwValue(i,	2, m_pFullSensorRecord->m_Hysteresis);
		SetDwValue(i,	3, m_pFullSensorRecord->m_ThresholdFlag);
		SetDwValue(i++,	4, m_pFullSensorRecord->m_EventMessage);

		SetDwValue(i++,	m_pFullSensorRecord->m_SensorType);
		SetDwValue(i++,	m_pFullSensorRecord->m_EventType);

		for(int j = 14; j>=0; j--)
		{
			SetDwValue(i, 14-j, BIT(m_pFullSensorRecord->m_LowerThresholdMask, j));
		}
		i++;

		for(int j = 14; j>=0; j--)
		{
			SetDwValue(i, 14-j, BIT(m_pFullSensorRecord->m_UpperThresholdMask, j));
		}
		i++;

		for(int j = 15; j>=0; j--)
		{
			SetDwValue(i, 15-j, BIT(m_pFullSensorRecord->m_ReadingMask, j));
		}
		i++;

		Unit1* pUnit1 = (Unit1*)&m_pFullSensorRecord->m_Uint1;
		SetDwValue(i,	0, pUnit1->m_isSigned);
		SetDwValue(i,	1, pUnit1->m_RateUnit);
		SetDwValue(i,	2, pUnit1->m_ModifyUnitFlag);
		SetDwValue(i++, 3, pUnit1->m_Percentage);

		SetDwValue(i++, m_pFullSensorRecord->m_BaseUnit);
		SetDwValue(i++, m_pFullSensorRecord->m_ModifyUnit);
		SetDwValue(i++, m_pFullSensorRecord->m_Linearization);

		SetDwValue(i++, (m_pFullSensorRecord->m_MMs<<8) | m_pFullSensorRecord->m_MLs);
		SetDwValue(i++, m_pFullSensorRecord->m_Tolerance);

		SetDwValue(i++, (m_pFullSensorRecord->m_BMs<<8) | m_pFullSensorRecord->m_BLs);
		SetDwValue(i++, (m_pFullSensorRecord->m_AccuracyMs<<6) | m_pFullSensorRecord->m_AccuracyLs);

		SetDwValue(i++, m_pFullSensorRecord->m_AccuracyExp);
		SetDwValue(i++, m_pFullSensorRecord->m_SensorDirection);
		SetDwValue(i++, m_pFullSensorRecord->m_Rexp);
		SetDwValue(i++, m_pFullSensorRecord->m_Bexp);

		SetDwValue(i,	0, m_pFullSensorRecord->m_IsMinSpecified);
		SetDwValue(i,	1, m_pFullSensorRecord->m_IsMaxSpecified);
		SetDwValue(i++, 2, m_pFullSensorRecord->m_IsReadingSpecified);

		SetDwValue(i++, m_pFullSensorRecord->m_NormalReading);
		SetDwValue(i++, m_pFullSensorRecord->m_NormalMax);
		SetDwValue(i++, m_pFullSensorRecord->m_NormalMin);
		SetDwValue(i++, m_pFullSensorRecord->m_MaxReading);
		SetDwValue(i++, m_pFullSensorRecord->m_MinReading);

		SetDwValue(i++, m_pFullSensorRecord->m_UpperNonRecoverable);
		SetDwValue(i++, m_pFullSensorRecord->m_UpperCiritcal);
		SetDwValue(i++, m_pFullSensorRecord->m_UpperNonCiritcal);
		SetDwValue(i++, m_pFullSensorRecord->m_LowerNonRecoverable);
		SetDwValue(i++, m_pFullSensorRecord->m_LowerCiritcal);
		SetDwValue(i++, m_pFullSensorRecord->m_LowerNonCiritcal);
		SetDwValue(i++, m_pFullSensorRecord->m_PoHysteresis);
		SetDwValue(i++, m_pFullSensorRecord->m_NeHysteresis);
		SetDwValue(i++, m_pFullSensorRecord->m_Oem);

		SetDwValue(i,	0, m_pFullSensorRecord->m_IdStrType);
		SetDwValue(i++,	1, m_pFullSensorRecord->m_IdStrLen);

		m_nStrFieldIndex = i;
		CString str(m_pFullSensorRecord->m_IdString);
		SetStrValue(i++, str);

		CMFCPropertyGridPropertyEx* pProp = GetItemByName(L"Discrete Reading Mask / Settable Threshold Mask");
		pProp->GetSubItem(1)->Enable(m_pFullSensorRecord->m_EventType != ET_THRESHOLD);
		pProp->GetSubItem(8)->Enable(m_pFullSensorRecord->m_EventType != ET_THRESHOLD);
		pProp->GetSubItem(9)->Enable(m_pFullSensorRecord->m_EventType != ET_THRESHOLD);
	}

	m_FruDataLen = sizeof(FullSensorRecord) 
			- sizeof(m_pFullSensorRecord->m_IdString) 
			+ m_pFullSensorRecord->m_IdStrLen
			+ sizeof(RecordHeader);

	return TRUE;
}

void CPropSdrFullSensor::AddSubObj(int nStart, int nCount)
{
}


void CPropSdrFullSensor::OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue)
{
	if(wcscmp(pChangedProp->GetName(), L"Event/Reading Type Code") == 0)	//P2pP AMC Resource Desc Count
	{
		CMFCPropertyGridPropertyEx* pProp = GetItemByName(L"Discrete Reading Mask / Settable Threshold Mask");
		ASSERT(pProp);
		DWORD dwNew = pChangedProp->GetByteValue();

		pProp->GetSubItem(1)->Enable(dwNew != ET_THRESHOLD);
		pProp->GetSubItem(8)->Enable(dwNew != ET_THRESHOLD);
		pProp->GetSubItem(9)->Enable(dwNew != ET_THRESHOLD);
		if(dwNew == ET_THRESHOLD)
		{
			pProp->GetSubItem(1)->SetValue((_variant_t)0);
			pProp->GetSubItem(8)->SetValue((_variant_t)0);
			pProp->GetSubItem(9)->SetValue((_variant_t)0);
		}
	}

	CMFCPropertyGridPropertyEx::OnPropertyChanged( pParentProp, pChangedProp, olderValue);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define OPT_ELF_HEADER 		0x1
#define OPT_SEC_HEADER 		0x2
#define OPT_PRO_HEADER 		0x4

// e_type member
#define ET_NONE 			0
#define ET_REL 				1
#define ET_EXEC 			2
#define ET_DYN 				3
#define ET_CORE				4
#define ET_LOOS				0xFE00
#define ET_HIOS				0xFEFF
#define ET_LOPROC			0xFF00
#define ET_HIPROC			0xFFFF

// e_machine member
#define EM_NONE				0
#define EM_M32				1
#define EM_SPARC			2
#define EM_386				3
#define EM_68K				4
#define EM_88K				5
#define EM_IAMCU			6
#define EM_860				7
#define EM_MIPS				8
#define EM_S370				9
#define EM_MIPS_RS3_LE		10
// RESERVED					11-	14
#define EM_PARISC			15
// RESERVED					16
#define EM_VPP500			17
#define EM_SPARC32PLUS		18
#define EM_960				19
#define EM_PPC				20
#define EM_PPC64			21
#define EM_S390				22
#define EM_SPU				23
// RESERVED					24-35
#define EM_V800				36
#define EM_FR20				37
#define EM_RH32				38
#define EM_RCE				39
#define EM_ARM				40
#define EM_ALPHA			41
#define EM_SH				42
#define EM_SPARCV9			43
#define EM_TRICORE			44
#define EM_ARC				45
#define EM_H8_300			46
#define EM_H8_300H			47
#define EM_H8S				48
#define EM_H8_500			49
#define EM_IA_64			50
#define EM_MIPS_X			51
#define EM_COLDFIRE			52
#define EM_68HC12			53
#define EM_MMA				54
#define EM_PCP				55
#define EM_NCPU				56
#define EM_NDR1				57
#define EM_STARCORE			58
#define EM_ME16				59
#define EM_ST100			60
#define EM_TINYJ			61
#define EM_X86_64			62
#define EM_PDSP				63
#define EM_PDP10			64
#define EM_PDP11			65
#define EM_FX66				66
#define EM_ST9PLUS			67
#define EM_ST7				68
#define EM_68HC16			69
#define EM_68HC11			70
#define EM_68HC08			71
#define EM_68HC05			72
#define EM_SVX				73
#define EM_ST19				74
#define EM_VAX				75
#define EM_CRIS				76
#define EM_JAVELIN			77
#define EM_FIREPATH			78
#define EM_ZSP				79
#define EM_MMIX				80
#define EM_HUANY			81
#define EM_PRISM			82
#define EM_AVR				83
#define EM_FR30				84
#define EM_D10V				85
#define EM_D30V				86
#define EM_V850				87
#define EM_M32R				88
#define EM_MN10300			89
#define EM_MN10200			90
#define EM_PJ				91
#define EM_OPENRISC			92
#define EM_ARC_COMPACT		93
#define EM_XTENSA			94
#define EM_VIDEOCORE		95
#define EM_TMM_GPP			96
#define EM_NS32K			97
#define EM_TPC				98
#define EM_SNP1K			99
#define EM_ST200			100
#define EM_IP2K				101
#define EM_MAX				102
#define EM_CR				103
#define EM_F2MC16			104
#define EM_MSP430			105
#define EM_BLACKFIN			106
#define EM_SE_C33			107
#define EM_SEP				108
#define EM_ARCA				109
#define EM_UNICORE			110
#define EM_EXCESS			111
#define EM_DXP				112
#define EM_ALTERA_NIOS2 	113
#define EM_CRX				114
#define EM_XGATE			115
#define EM_C166				116
#define EM_M16C				117
#define EM_DSPIC30F			118
#define EM_CE				119
#define EM_M32C				120
// RESERVED					121-130
#define EM_TSK3000			131
#define EM_RS08				132
#define EM_SHARC			133
#define EM_ECOG2			134
#define EM_SCORE7			135
#define EM_DSP24			136
#define EM_VIDEOCORE3		137
#define EM_LATTICEMICO32	138
#define EM_SE_C17			139
#define EM_TI_C6000			140
#define EM_TI_C2000			141
#define EM_TI_C5500			142
#define EM_TI_ARP32			143
#define EM_TI_PRU			144
// RESERVED					145-159
#define EM_MMDSP_PLUS		160
#define EM_CYPRESS_M8C		161
#define EM_R32C				162
#define EM_TRIMEDIA			163
#define EM_QDSP6			164
#define EM_8051				165
#define EM_STXP7X			166
#define EM_NDS32			167
#define EM_ECOG1			168
#define EM_ECOG1X			168
#define EM_MAXQ30			169
#define EM_XIMO16			170
#define EM_MANIK			171
#define EM_CRAYNV2			172
#define EM_RX				173
#define EM_METAG			174
#define EM_MCST_ELBRUS		175
#define EM_ECOG16			176
#define EM_CR16				177
#define EM_ETPU				178
#define EM_SLE9X			179
#define EM_L10M				180
#define EM_K10M				181
// RESERVED					182
#define EM_AARCH64			183
// RESERVED					184
#define EM_AVR32			185
#define EM_STM8				186
#define EM_TILE64			187
#define EM_TILEPRO			188
#define EM_MICROBLAZE		189
#define EM_CUDA				190
#define EM_TILEGX			191
#define EM_CLOUDSHIELD		192
#define	EM_COREA_1ST		193
#define EM_COREA_2ND		194
#define EM_ARC_COMPACT2		195
#define EM_OPEN8			196
#define EM_RL78				197
#define EM_VIDEOCORE5		198
#define EM_78KOR			199
#define EM_56800EX			200
#define EM_BA1				201
#define EM_BA2				202
#define EM_XCORE			203
#define EM_MCHP_PIC			204
#define EM_INTEL205			205
#define EM_INTEL206			206
#define EM_INTEL207			207
#define EM_INTEL208			208
#define EM_INTEL209			209
#define EM_KM32				210
#define EM_KMX32			211
#define EM_KMX16			212
#define EM_KMX8				213
#define EM_KVARC			214
#define EM_CDP				215
#define EM_COGE				216
#define EM_COOL				217
#define EM_NORC				218
#define EM_CSR_KALIMBA		219
#define EM_Z80				220
#define EM_VISIUM			221
#define EM_FT32				222
#define EM_MOXIE			223
#define EM_AMDGPU			224
// RESERVED					225-242
#define EM_RISCV			243

#define EI_NIDENT 			16

// e_version
#define EV_NONE				0			// Invalid version
#define EV_CURRENT			1			// Current version

// e_ident[] Identification Indexs
#define EI_MAG0				0			// File identification
#define EI_MAG1				1			// File identification
#define EI_MAG2				2			// File identification
#define EI_MAG3				3			// File identification
#define EI_CLASS			4			// File Class
#define EI_DATA				5			// Data encoding
#define EI_VERSION			6			// File version
#define EI_OSABI			7			// Operating system/ABI identification
#define EI_ABIVERSION		8			// ABI version
#define EI_PAD				9			// Start of padding bytes
#define EI_NIDENT			16			// Size of e_ident[]

// e_ident[EI_CLASS]
#define ELFCLASSNONE		0			// Invalid class
#define ELFCLASS32			1			// 32-bit object
#define ELFCLASS64			2			// 64-bit object

// e_ident[EI_DATA]							
#define ELFDATANONE			0			// Invalid data encoding
#define ELFDATA2LSB			1			// 2's complement values, with the leaset significant byte occupying the lowest address
#define ELFDATA2MSB			2			// 2's complement values, with the most significant byte occuping the lowest address

// e_ident[EI_OSABI]
#define ELFOSABI_NONE		0			
#define ELFOSABI_HPUX		1			
#define ELFOSABI_NETBSD		2
#define ELFOSABI_GNU		3
#define ELFOSABI_LINUX		3
#define ELFOSABI_SOLARIS	6
#define ELFOSABI_AIX		7
#define ELFOSABI_IRIX		8
#define ELFOSABI_FREEBSD	9
#define ELFOSABI_TRU64		10
#define ELFOSABI_MODESTO	11
#define ELFOSABI_OPENBSD	12
#define ELFOSABI_OPENVMS	13
#define ELFOSABI_NSK		14
#define ELFOSABI_AROS		15
#define ELFOSABI_FENIXOS	16
#define ELFOSABI_CLOUDABI	17
#define ELFOSABI_OPENVOS	18
//							64-255		// Architecture-specific value range

// 32-bit ELF base types.
typedef unsigned int Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Off;
typedef signed int Elf32_Sword;
typedef unsigned int Elf32_Word;

// 64-bit ELF base types.
typedef unsigned long long Elf64_Addr;
typedef unsigned short Elf64_Half;
typedef signed short Elf64_SHalf;
typedef unsigned long long Elf64_Off;
typedef unsigned int Elf64_Sword;
typedef unsigned int Elf64_Word;
typedef unsigned long long Elf64_Xword;
typedef signed long long Elf64_Sxword;

// 32-bit Elf filed header
typedef struct elf32_hdr{
	unsigned char e_ident[EI_NIDENT];
	Elf32_Half 	e_type;
	Elf32_Half 	e_machine;
	Elf32_Word 	e_version;
	Elf32_Addr 	e_entry;
	Elf32_Off 	e_phoff;
	Elf32_Off 	e_shoff;
	Elf32_Word 	e_flags;
	Elf32_Half	e_ehsize;
	Elf32_Half	e_phentsize;
	Elf32_Half	e_phnum;
	Elf32_Half	e_shentsize;
	Elf32_Half	e_shnum;
	Elf32_Half	e_shstrndx;
} Elf32_Ehdr;

typedef struct elf32_shdr{
	Elf32_Word	sh_name;
	Elf32_Word	sh_type;
	Elf32_Word	sh_flags;
	Elf32_Addr	sh_addr;
	Elf32_Off	sh_offset;
	Elf32_Word	sh_size;
	Elf32_Word	sh_link;
	Elf32_Word	sh_info;
	Elf32_Word	sh_addralign;
	Elf32_Word	sh_entsize;
} Elf32_Shdr;

typedef struct elf32_phdr{
	Elf32_Word	p_type;
	Elf32_Off	p_offset;
	Elf32_Addr	p_vaddr;
	Elf32_Addr	p_paddr;
	Elf32_Word	p_filesz;
	Elf32_Word	p_memsz;
	Elf32_Word	p_flags;
	Elf32_Word	p_align;
} Elf32_Phdr;
	
typedef struct elf64_hdr{
	unsigned char 	e_ident[EI_NIDENT];	// Elf identification
	Elf64_Half		e_type;				// Object file
	Elf64_Half		e_machine;			// Machine type
	Elf64_Word		e_version;			// Object file version
	Elf64_Addr		e_entry;			// Entry point address
	Elf64_Off		e_phoff;			// Program header offset
	Elf64_Off		e_shoff;			// Section header offset
	Elf64_Word		e_flags;			// Processor-specific flags
	Elf64_Half		e_ehsize;			// Elf header size
	Elf64_Half		e_phentsize;		// Size of program header entry
	Elf64_Half		e_phnum;			// Number of program header entries
	Elf64_Half		e_shentsize;		// Size of section header entry
	Elf64_Half		e_shnum;			// Number of section header entries
	Elf64_Half		e_shstrndx;			// Section name string table index
} Elf64_Ehdr;

// Special Section Indexes
#define SHN_UNDEF					0
#define SHN_LORESERVE				0xff00
#define SHN_LOPROC					0xff00
#define SHN_HIPROC					0xff1f
#define SHN_LOOS					0xff20
#define SHN_HIOS					0xff3f
#define SHN_ABS						0xfff1
#define SHN_COMMON					0xfff2
#define SHN_XINDEX					0xffff
#define SHN_HIRESERVE				0xffff

// Section types
#define SHT_NULL					0
#define SHT_PROGBITS				1	
#define SHT_SYMTAB					2
#define SHT_STRTAB					3
#define SHT_RELA					4
#define SHT_HASH					5
#define SHT_DYNAMIC					6
#define SHT_NOTE					7
#define SHT_NOBITS					8
#define SHT_REL						9
#define SHT_SHLIB					10
#define SHT_DYNSYM					11
#define SHT_INIT_ARRAY				14
#define SHT_FINI_ARRAY				15
#define SHT_PREINIT_ARRAY			16
#define SHT_GROUP					17
#define SHT_SYMTAB_SHNDX			18
#define SHT_LOOS					0x60000000
#define SHT_HIOS					0x6fffffff
#define SHT_LOPROC					0x70000000
#define SHT_HIPROC					0x7fffffff
#define SHT_LOUSER					0x80000000
#define SHT_HIUSER					0xffffffff

// Section Attribute Flags
#define SHF_WRITE 					0x1
#define SHF_ALLOC 					0x2
#define SHF_EXECINSTR 				0x4
#define SHF_MERGE					0x10
#define SHF_STRINGS					0x20
#define SHF_INFO_LINK				0x40
#define SHF_LINK_ORDER				0x80
#define SHF_OS_NONCONFORMING		0x100
#define SHF_GROUP					0x200
#define SHF_TLS						0x400
#define SHF_COMPRESSED				0x800
#define SHF_MASKOS					0x0ff00000
#define SHF_MASKPROC				0xf0000000

typedef struct elf64_shdr{
	Elf64_Word		sh_name;		// Section name
	Elf64_Word		sh_type;		// Section type
	Elf64_Xword		sh_flags;		// Section attributes
	Elf64_Addr		sh_addr;		// Virtual address in memory
	Elf64_Off		sh_offset;		// Offset in file
	Elf64_Xword		sh_size;		// size of section
	Elf64_Word		sh_link;		// Link to other section
	Elf64_Word		sh_info;		// Miscellaneous information
	Elf64_Xword		sh_addralign;	// Address alignment boundary
	Elf64_Xword		sh_entsize;		// Size of entries, if section has table
} Elf64_Shdr;

// segment type
#define PT_NULL						0
#define PT_LOAD						1
#define PT_DYNAMIC					2
#define PT_INTERP					3
#define PT_NOTE						4
#define PT_SHLIB					5
#define PT_PHDR						6
#define PT_TLS						7
#define PT_LOOS						0x60000000
#define PT_HIOS						0x6fffffff
#define PT_LOPROC					0x70000000
#define PT_HIPROC					0x7fffffff

typedef struct elf64_phdr{
	Elf64_Word		p_type;			// type of Segment
	Elf64_Word		p_flags;		// Segment attributes
	Elf64_Off		p_offset;		// Offset in file
	Elf64_Addr		p_vaddr;		// vitual address in memory
	Elf64_Addr		p_paddr;		// Reserved
	Elf64_Xword		p_filesz;		// Size of segment in file
	Elf64_Xword		p_memsz;		// Size of segment in memory
	Elf64_Xword		p_align;		// Alignment of segment
}Elf64_Phdr;

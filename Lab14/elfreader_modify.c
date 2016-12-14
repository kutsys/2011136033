#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define OPT_ELF_HEADER 	0x1
#define OPT_PRO_HEADER 	0x2
#define OPT_SEC_HEADER 	0x4

// e_type member
#define ET_NONE 		0
#define ET_REL 			1
#define ET_EXEC 		2
#define ET_DYN 			3
#define ET_CORE			4
#define ET_LOOS			0xFE00
#define ET_HIOS			0xFEFF
#define ET_LOPROC		0xFF00
#define ET_HIPROC		0xFFFF

// e_machine member
#define EM_NONE			0
#define EM_M32			1
#define EM_SPARC		2
#define EM_386			3
#define EM_68K			4
#define EM_88K			5
#define EM_IAMCU		6
#define EM_860			7
#define EM_MIPS			8
#define EM_S370			9
#define EM_MIPS_RS3_LE	10
// RESERVED				11-14
#define EM_PARISC		15
// RESERVED				16
#define EM_VPP500		17
#define EM_SPARC32PLUS	18
#define EM_960			19
#define EM_PPC			20
#define EM_PPC64		21
#define EM_S390			22
#define EM_SPU			23
// RESERVED				24-35
#define EM_V800			36
#define EM_FR20			37
#define EM_RH32			38
#define EM_RCE			39
#define EM_ARM			40
#define EM_ALPHA		41
#define EM_SH			42
#define EM_SPARCV9		43
#define EM_TRICORE		44
#define EM_ARC			45
#define EM_H8_300		46
#define EM_H8_300H		47
#define EM_H8S			48
#define EM_H8_500		49
#define EM_IA_64		50
#define EM_MIPS_X		51
#define EM_COLDFIRE		52
#define EM_68HC12		53
#define EM_MMA			54
#define EM_PCP			55
#define EM_NCPU			56
#define EM_NDR1			57
#define EM_STARCORE		58
#define EM_ME16			59
#define EM_ST100		60
#define EM_TINYJ		61
#define EM_X86_64		62
#define EM_PDSP			63
#define EM_PDP10		64
#define EM_PDP11		65
#define EM_FX66			66
#define EM_ST9PLUS		67
#define EM_ST7			68
#define EM_68HC16		69
#define EM_68HC11		70
#define EM_68HC08		71
#define EM_68HC05		72
#define EM_SVX			73
#define EM_ST19			74
#define EM_VAX			75
#define EM_CRIS			76
#define EM_JAVELIN		77
#define EM_FIREPATH		78
#define EM_ZSP			79
#define EM_MMIX			80
#define EM_HUANY		81
#define EM_PRISM		82
#define EM_AVR			83
#define FR30			84
#define EM_D10V			85
#define EM_D30V			86
#define EM_V850			87
#define EM_M32R			88
#define EM_MN10300		89
#define EM_MN10200		90
#define EM_PJ			91
#define EM_OPENRISC		92
#define EM_ARC_COMPACT	93
#define EM_XTENSA		94
#define EM_VIDEOCORE	95
#define EM_TMM_GPP		96
#define EM_NS32K		97
#define EM_TPC			98
#define EM_SNP1K		99
#define EM_ST200		100
#define EM_IP2K			101
#define EM_MAX			102
#define EM_CR			103
#define EM_F2MC16		104
#define EM_MSP430		


#define EI_NIDENT 		16

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

int main(int argc, char **argv){
	int c, opt = 0;
	char *exec = NULL;
	File *fp =NULL;
	char *section_name;
	Elf64_Ehdr elfEhdr;
	Elf64_Shdr elfShdr;
	Elf64_Phdr elfPhdr;

	while((c = getopt(argc, argv, "eps")) != -1)
		switch(c){
			case 'e':
				opt |= OPT_ELF_HEADER;
				break;
			case 'p':
				opt |= OPT_PRO_HEADER;
				break;
			case 's':
				opt |= OPT_SEC_HEADER;
				break;
			case '?':
				if (isprint(optopt))
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
				return 1;
			default:
				abort();
		}
	exec = argv[optind];

	if(exec == NULL){
		fprintf(stderr, "input file error\n");
		return -1;
	}
	
	fp = fopen(exec, "r");
	if(fp == NULL){
		fprintf(stderr, "Could not open file %s\n", exec);
		return -1;
	}
	fread(&elfEhdr, 1, sizeof(elfEhdr), fp);	// read elf file header

	if(opt == 0 || (opt & OPT_ELF_HEADER)){
		printf("ELF Header:\n");
		printf("\tMagic:\t\t%s\n", elfEhdr.e_ident);
		printf("\tType:\t\t");
		switch(elfEhdr.e_type){
			case ET_NONE:
				printf("No file type");
				break;
			case ET_REL:
				pritnf("Relocatable object file");
				break;
			case ET_EXEC:
				printf("Executable file");
				break;
			case ET_DYN:
				printf("Shared object file");
				break;
			case ET_CORE:
				printf("Core file");
				break;
			case ET_LOOS:
			case ET_HIOS:
				printf("Environment_specific use");
				break;
			case ET_LOPROC:
			case ET_HIPROC:
				printf("Processor-specific use");
				break;
		}
		printf("\n");
		printf("\tMachine:\t\t");
		siwtch(elfEhdr.e_machine){
		case 0:										// EM_NONE
			printf("No machine");
			break;
		case 1:										// EM_M32
			printf("AT&T WE 32100");
			break;
		case 2:										// EM_SPARC
			printf("SPARC");
			break;
		case 3:										// EM_386
			printf("Intel 80386");
			break;
		case 
		}

	}
	if(opt & OPT_PRO_HEADER){
		
	}
	if(opt & OPT_SEC_HEADER){

	}

	fclose(fp);
	return 0;
}

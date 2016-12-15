#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "elfreader.h"

int main(int argc, char **argv){
	int c, opt = 0;
	char *exec = NULL, magic[EI_NIDENT];
	int fClass = 0;
	FILE *fp =NULL;
	int i;

	while((c = getopt(argc, argv, "eps")) != -1)
		switch(c){
			case 'e':
				opt |= OPT_ELF_HEADER; break;
			case 'p':
				opt |= OPT_PRO_HEADER; break;
			case 's':
				opt |= OPT_SEC_HEADER; break;
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

	if(exec == NULL){fprintf(stderr, "input file error\n"); return -1;}
	
	fp = fopen(exec, "r");
	if(fp == NULL){fprintf(stderr, "Could not open file %s\n", exec);return -1;}

	fread(&magic, 1, sizeof(magic), fp);		// read magic character

	if(magic[EI_MAG0] != 127|| strncmp(magic+1, "ELF", 3)){
		printf("%s\n", magic);
		fprintf(stderr, "this file is not elf file.\n"); 
		return -1;}

	fseek(fp, 0, SEEK_SET);
	if(magic[EI_CLASS] == ELFCLASS32){
		Elf32_Ehdr elfEhdr;
		Elf32_Shdr elfShdr;
		Elf32_Phdr elfPhdr;

		fread(&elfEhdr, 1, sizeof(elfEhdr), fp);	// read elf file header
		if(opt == 0 || (opt & OPT_ELF_HEADER)){
			printf("ELF Header:\n");
			printf(" Magic: ");						// printf magic number
			for(i=0; i<EI_NIDENT; i++)
				printf("%02x ", elfEhdr.e_ident[i]);
			printf("\n");
			
			// analyze magic code
			printf(" Class: 32-bit objects\n");		// Class always is 32-bit objects
			printf(" Data: ");						
			switch(elfEhdr.e_ident[EI_DATA]){
				case ELFDATANONE:
					printf("Invalid data encoding"); break;
				case ELFDATA2LSB:
					printf("2's complement, little endian"); break;
				case ELFDATA2MSB:
					printf("2's complement, big endian"); break;
			}
			printf("\n");
			printf(" Version: ");
			switch(elfEhdr.e_ident[EI_VERSION]){
				case EV_NONE:
					printf("Invalid version");break;
				case EV_CURRENT:
					printf("Current version");break;
			}
			printf("\n");
			printf(" OS/ABI: ");
			switch(elfEhdr.e_ident[EI_OSABI]){
				case ELFOSABI_NONE:
					printf("No extension or unspecified");break;
				case ELFOSABI_HPUX:
					printf("Hewlett-Packard HP-UX");break;
				case ELFOSABI_NETBSD:
					printf("NetBSD");break;
				//case ELFOSABI_GNU:		//this value is same with ELFOSABI_LINUX
				//	printf("GNU");break;
				case ELFOSABI_LINUX:
					printf("Linux");break;
				case ELFOSABI_SOLARIS:
					printf("Sun Solaris");break;
				case ELFOSABI_AIX:
					printf("AIX");break;
				case ELFOSABI_IRIX:
					printf("IRIX");break;
				case ELFOSABI_FREEBSD:
					printf("FreeBSD");break;
				case ELFOSABI_TRU64:
					printf("Compaq TRU64 UNIX");break;
				case ELFOSABI_MODESTO:
					printf("Novell Modesto");break;
				case ELFOSABI_OPENBSD:
					printf("Open BSD");break;
				case ELFOSABI_OPENVMS:
					printf("Open VMS");break;
				case ELFOSABI_NSK:
					printf("Hewlett-Packard Non-Stop Kernel");break;
				case ELFOSABI_AROS:
					printf("Amiga Research OS");break;
				case ELFOSABI_FENIXOS:
					printf("The FenixOS highly scalable multi-core OS");break;
				case ELFOSABI_CLOUDABI:
					printf("Nuxi CloudABI");break;
				case ELFOSABI_OPENVOS:
					printf("Stratus Technologies OpenVOS");break;
			}
			printf("\n");
			printf(" ABI Version: ");
			switch(elfEhdr.e_ident[EI_ABIVERSION]){
				case EV_NONE:
					printf("invalid version");break;
				case EV_CURRENT:
					printf("Current version");break;
			}
			printf("\n");
			printf(" Type: ");
			switch(elfEhdr.e_type){
				case ET_NONE:
					printf("No file type");break;
				case ET_REL:
					printf("Relocatable object file");break;
				case ET_EXEC:
					printf("Executable file");break;
				case ET_DYN:
					printf("Shared object file");break;
				case ET_CORE:
					printf("Core file");break;
				case ET_LOOS:
				case ET_HIOS:
					printf("Environment_specific use");break;
				case ET_LOPROC:
				case ET_HIPROC:
					printf("Processor-specific use");break;
			}
			printf("\n");
			printf(" Machine: ");
			switch(elfEhdr.e_machine){
				case EM_NONE:
					printf("No machine");break;
				case EM_M32:
					printf("AT&T WE 32100");break;
				case EM_SPARC:
					printf("SPARC");break;
				case EM_386:
					printf("Intel 80386");break;
				case EM_68K:
					printf("Motorola 68000");break;
				case EM_88K:
					printf("Motorola 88000");break;
				case EM_IAMCU:
					printf("Intel MCU"); break;
				case EM_860:
					printf("Intel 80860");break;
				case EM_MIPS:
					printf("MIPS I Architecture");break;
				case EM_S370:
					printf("IBM System/370 Processor");break;
				case EM_MIPS_RS3_LE:
					printf("MIPS RS3000 Little-endian");break;
				case EM_PARISC:
					printf("Hewlett-Packard PA-RISC");break;
				case EM_VPP500:
					printf("Fujitsu VPP500");break;
				case EM_SPARC32PLUS:
					printf("Enhanced instruction set SPARC");break;
				case EM_960:
					printf("Intel 80960");break;
				case EM_PPC:
					printf("Power PC");break;
				case EM_PPC64:
					printf("64-bit Power PC");break;
				case EM_S390:
					printf("IBM System/390 Processor");break;
				case EM_SPU:
					printf("IBM SPU/SPC");break;
				case EM_V800:
					printf("NEC V800");break;
				case EM_FR20:
					printf("Fujitsu FR20");break;
				case EM_RH32:
					printf("TRW RH-32");break;
				case EM_RCE:
					printf("Motorola RCE");break;
				case EM_ARM:
					printf("ARM 32-bit architecture(AARCH32)");break;
				case EM_ALPHA:
					printf("Digital Alpha");break;
				case EM_SH:
					printf("Hitachi SH"); break;
				case EM_SPARCV9:
					printf("SPARC Version 9"); break;
				case EM_TRICORE:
					printf("Siemens TriCore embedded processor");break;
				case EM_ARC:
					printf("Argonaut RISC Core, Argonaut Technologies Inc.");break;
				case EM_H8_300:
					printf("Hitachi H8/300"); break;
				case EM_H8_300H:
					printf("Hitachi H8/300H");break;
				case EM_H8S:
					printf("Hitachi H8S");break;
				case EM_H8_500:
					printf("Hitachi H8/500");break;
				case EM_IA_64:
					printf("Intel IA-64 Processor architecture");break;
				case EM_MIPS_X:
					printf("Stanford MIPS-X");break;
				case EM_COLDFIRE:
					printf("Motorola ColdFire");break;
				case EM_68HC12:
					printf("Motorola M68HC12");break;
				case EM_MMA:
					printf("Fujitus MMA Multimedia Accelerator");break;
				case EM_PCP:
					printf("Siemens PCP");break;
				case EM_NCPU:
					printf("Sony nCPU embedded RISC Processor");break;
				case EM_NDR1:
					printf("Denso NDR1 microprocessor");break;
				case EM_STARCORE:
					printf("Motorola Star*Core Processor");break;
				case EM_ME16:
					printf("Toyota ME16 Processor");break;
				case EM_ST100:
					printf("STMicroelectronics ST100 processor");break;
				case EM_TINYJ:
					printf("Advanced Logic Corp. TinyJ embedded processor famlity");break;
				case EM_X86_64:
					printf("AMD x86-64 architecture");break;
				case EM_PDSP:
					printf("Sony DSP Processor");break;
				case EM_PDP10:
					printf("Digital Equipment Corp.PDP-10");break;
				case EM_PDP11:
					printf("Digital Equipment Corp.PDP-11");break;
				case EM_FX66:
					printf("Siemens FX66 microcontorller");break;
				case EM_ST9PLUS:
					printf("STMicroelectronics ST9+ 8/16 bit microcontroller");break;
				case EM_ST7:
					printf("StMicroelectronics ST7 6-bit microcontroller"); break;
				case EM_68HC16:
					printf("Motorola MC68HC16 Microcontroller");break;
				case EM_68HC11:
					printf("Motorola MC68HC11 Microcontroller");break;
				case EM_68HC08:
					printf("Motorola MC68HC08 Microcontroller");break;
				case EM_68HC05:
					printf("Motorola MC68HC05 Microcontroller");break;
				case EM_SVX:
					printf("Silicon Graphics SVx");break;
				case EM_ST19:
					printf("STMicroelectronics ST19 8-bit microcontroller");break;
				case EM_VAX:
					printf("Digital VAX");break;
				case EM_CRIS:
					printf("Axis Communication 32-bit embedded processor");break;
				case EM_JAVELIN:
					printf("Infineon Technologies 32-bit embedded processor");break;
				case EM_FIREPATH:
					printf("Element 14 64-bit DSP processor");break;
				case EM_ZSP:
					printf("LSI Logic 16-bit DSP Processor");break;
				case EM_MMIX:
					printf("Donald Knuth's educational 64-bit processor");break;
				case EM_HUANY:
					printf("Harvard University machine-independent object file");break;
				case EM_PRISM:
					printf("Sitera Prism");break;
				case EM_AVR:
					printf("Atmel AVR 8-bit microcontroller");break;
				case EM_FR30:
					printf("Fujitus FR30");break;
				case EM_D10V:
					printf("Mitsubishi D10V");break;
				case EM_D30V:
					printf("Mitsubishi D30V");break;
				case EM_V850:
					printf("NEC v850");break;
				case EM_M32R:
					printf("Mitsubishi M32R");break;
				case EM_MN10300:
					printf("Matsushita MN10300");break;
				case EM_MN10200:
					printf("Matsushita MN10200");break;
				case EM_PJ:
					printf("picoJava");break;
				case EM_OPENRISC:
					printf("OpenRISC 32-bit embedded processor");break;
				case EM_ARC_COMPACT:
					printf("ARC International ARCompact processor (old spelling/synonym: EM_ARC_A5)");break;
				case EM_XTENSA:
					printf("Tensilica Xtensa Architecture");break;
				case EM_VIDEOCORE:
					printf("Alphamosaic VideoCore Processor");break;
				case EM_TMM_GPP:
					printf("Thompson Multimedia General Purpose Processor");break;
				case EM_NS32K:
					printf("National Semiconductor 32000 series");break;
				case EM_TPC:
					printf("Tenor Network TPC processor");break;
				case EM_SNP1K:
					printf("Trebia SNP 1000 Processor");break;
				case EM_ST200:
					printf("STMicroelectronics (www.st.com) ST200 microcontroller");break;
				case EM_IP2K:
					printf("Ubicom IP2xxx microcontroller family");break;
				case EM_MAX:
					printf("MAX Processor");break;
				case EM_CR:
					printf("National Semiconductor CompactRISC microprocessor");break;
				case EM_F2MC16:
					printf("Fujitsu F2MC16");break;
				case EM_MSP430:
					printf("Texas Instrucments embedded microcontroller msp430");break;
				case EM_BLACKFIN:
					printf("Analog Devices Blackfin(DSP) processor");break;
				case EM_SE_C33:
					printf("S1C33 Family of Seiko Epson processor");break;
				case EM_SEP:
					printf("Sharp embedded microprocessor");break;
				case EM_ARCA:
					printf("Arca RISC microprocessor");break;
				case EM_UNICORE:
					printf("Microprocessor series from PKU_Unity Ltd. and MPRC of Peking University");break;
				case EM_EXCESS:
					printf("eXcess: 16/32/64-bit configurable embedded CPU");break;
				case EM_DXP:
					printf("Icera Semiconductor Inc. Deep Execution Processor");break;
				case EM_ALTERA_NIOS2:
					printf("Altera Nios 2 soft-core processor");break;
				case EM_CRX:
					printf("National Semiconductor CompactRISC CRX microprocessor");break;
				case EM_XGATE:
					printf("Motorola XGATE embedded processor");break;
				case EM_C166:
					printf("Infineon C16x/XC16x Processor");break;
				case EM_M16C:
					printf("Renesas M16C series microprocessors");break;
				case EM_DSPIC30F:
					printf("Microchip Technology dsPIC30F DIgital Signal Controller");break;
				case EM_CE:
					printf("Freescale Communication Engine RISC core");break;
				case EM_TSK3000:
					printf("Altium TSK3000 core");break;
				case EM_RS08:
					printf("Fresscale RS08 embedded processor");break;
				case EM_SHARC:
					printf("Analog Devices SHARC family of 32-bit DSP processors");break;
				case EM_ECOG2:
					printf("Cyan Tchnology eCOG2 microprocessor");break;
				case EM_SCORE7:
					printf("Sunplus S+core7 RISC processor");break;
				case EM_DSP24:
					printf("New Japan Radio(NJR) 24-bit DSP Processor");break;
				case EM_VIDEOCORE3:
					printf("Broadcom VeideoCore3 Processor");break;
				case EM_LATTICEMICO32:
					printf("RISC processor for Lattice FPGA architecture");break;
				case EM_SE_C17:
					printf("Seiko Epson C17 Family");break;
				case EM_TI_C6000:
					printf("The Texas Instruments TMS320C6000 DSP family");break;
				case EM_TI_C2000:
					printf("The Texas Instruments TMS320C2000 DSP family");break;
				case EM_TI_C5500:
					printf("The Texas Instruments TMS320C55x DSP family");break;
				case EM_TI_ARP32:
					printf("Texas Instruments Application Specific RISC Processor, 32bit fetch");break;
				case EM_TI_PRU:
					printf("Texas Instruments Programmable Runtime Unit");break;
				case EM_MMDSP_PLUS:
					printf("STMicroelectronics 64bits VLIW Data Signal Processor");break;
				case EM_CYPRESS_M8C:
					printf("Cypress M8C microprocessor");break;
				case EM_R32C:
					printf("Renesas R32C series microprocessors");break;
				case EM_TRIMEDIA:
					printf("NXP Semiconductors TriMedia architecture family");break;
				case EM_QDSP6:
					printf("QUALCOMM DSP6 Processor");break;
				case EM_8051:
					printf("Intel 8051 and variants");break;
				case EM_STXP7X:
					printf("STMicroelectronics STxP7x family of configurable and exensible RISC processors");break;
				case EM_NDS32:
					printf("Andes Technology compact code size embedded RISC processor family");break;
				case EM_ECOG1: // == case EM_ECOG1X
					printf("Cyan Technology eCOG1X family");break;
				case EM_MAXQ30:
					printf("Dallas Semiconductor MAXQ30 Core Micro-controllers");break;
				case EM_XIMO16:
					printf("New Japan Radio (NJR) 16-bit DSP Processor");break;
				case EM_MANIK:
					printf("M2000 REconfigutable RISC Microprocessor");break;
				case EM_CRAYNV2:
					printf("Cray Inc. NV2 vector Architecture");break;
				case EM_RX:
					printf("Renesas RX family");break;
				case EM_METAG:
					printf("Imagination Technologies META processor architecture");break;
				case EM_MCST_ELBRUS:
					printf("MCST Elbrus general purpose hardware architecture");break;
				case EM_ECOG16:
					printf("Cyan Technology eCOG16 family");break;
				case EM_CR16:
					printf("National Semiconductor CompactRISC CR16 16-bit microprocessor");break;
				case EM_ETPU:
					printf("Freescale Extended Time processing Unit");break;
				case EM_SLE9X:
					printf("Infineon TEchnologies SLE9X core");break;
				case EM_L10M:
					printf("Intel L10M");break;
				case EM_K10M:
					printf("Intel K10M");break;
				case EM_AARCH64:
					printf("ARM 64-bit architecture (AARCH64)");break;
				case EM_AVR32:
					printf("Atmel Corporation 32-bit microprocessor family");break;
				case EM_STM8:
					printf("STMicroelectronics STM8 8-bit microcontroller");break;
				case EM_TILE64:
					printf("Tilera TILE64 multicore acrchitecture family");break;
				case EM_TILEPRO:
					printf("Tilera TILEPro multicore architecture family");break;
				case EM_MICROBLAZE:
					printf("Xilinx MicroBlaze 32-bit RISC soft processor core");break;
				case EM_CUDA:
					printf("NVIDIA CUDA Architecture");break;
				case EM_TILEGX:
					printf("Tilera TILE-Gx multicore architecture family");break;
				case EM_CLOUDSHIELD:
					printf("CloudShield architecture family");break;
				case EM_COREA_1ST:
					printf("KIPO-KAIST Core-A 1st generation processor family");break;
				case EM_COREA_2ND:
					printf("KIPO-KAIST Core-A 2nd generation processor family");break;
				case EM_ARC_COMPACT2:
					printf("Synopsys ArCompcat V2");break;
				case EM_OPEN8:
					printf("Open8 8-bit RISC soft processor core");break;
				case EM_RL78:
					printf("Renesas RL78 family");break;
				case EM_VIDEOCORE5:
					printf("Broadcom VideoCore V processor");break;
				case EM_78KOR:
					printf("Renesas 78KOR family");break;
				case EM_56800EX:
					printf("Freescale 56800EX Digital Signal Controller (DSC)");break;
				case EM_BA1:
					printf("Beyond BA1 CPU Architecture");break;
				case EM_BA2:
					printf("Beyond BA2 CPU Architecture");break;
				case EM_XCORE:
					printf("XMOS xCORE processor family");break;
				case EM_MCHP_PIC:
					printf("Microchip 8-bit PIC(r) family");break;
				case EM_INTEL205:
				case EM_INTEL206:
				case EM_INTEL207:
				case EM_INTEL208:
				case EM_INTEL209:
					printf("Reserved by Intel");break;
				case EM_KM32:
					printf("KM211 KM32 32-bit processor");break;
				case EM_KMX32:
					printf("KM211 KMX32 32-bit processor");break;
				case EM_KMX16:
					printf("KM211 KMX16 16-bit processor");break;
				case EM_KMX8:
					printf("KM211 KMX8 8-bit processor");break;
				case EM_KVARC:
					printf("KM211 KVARC processor");break;
				case EM_CDP:
					printf("Paneve CDP architecture family");break;
				case EM_COGE:
					printf("Cognitive Smart Memory Processor");break;
				case EM_COOL:
					printf("Bluechip Systems CoolEngine");break;
				case EM_NORC:
					printf("Nanoradio OPtimized RISC");break;
				case EM_CSR_KALIMBA:
					printf("CSR Kalimba architecture family");break;
				case EM_Z80:
					printf("Zilog Z80");break;
				case EM_VISIUM:
					printf("Controls and Data services VISIUMcore processor");break;
				case EM_FT32:
					printf("FTDI chip FT32 high performance 32-bit RISC architecture");break;
				case EM_MOXIE:
					printf("Moxie processor family");break;
				case EM_AMDGPU:
					printf("AMD GPU architecture");break;
				case EM_RISCV:
					printf("RISC-V");break;
			}
			printf("\n");
			printf(" Version: ");
			switch(elfEhdr.e_version){
				case EV_NONE:
					printf("Invalid version");break;
				case EV_CURRENT:
					printf("Current version");break;
			}
			printf("\n");
			printf(" Etnry point address: 0x%x\n", elfEhdr.e_entry);
			printf(" Start of program header: %d (bytes into file)\n", elfEhdr.e_phoff);
			printf(" Start of Section header: %d (bytes into file)\n", elfEhdr.e_shoff);
			printf(" Flags: 0x%x\n", elfEhdr.e_flags);
			printf(" Size of this header: %d (btyes)\n", elfEhdr.e_ehsize);
			printf(" Size of program headers: %d (bytes)\n", elfEhdr.e_phentsize);
			printf(" Number of program headers: %d\n", elfEhdr.e_phnum);
			printf(" Size of Section headers: %d (bytes)\n", elfEhdr.e_shentsize);
			printf(" Number of Section headers: %d\n", elfEhdr.e_shnum);
			printf(" Section header string table index: %d\n", elfEhdr.e_shstrndx);
			printf("\n");
		}
		if(opt & OPT_SEC_HEADER){
			char *string_table;
			char section_type[16];
			// read string table
			fseek(fp, elfEhdr.e_shoff + elfEhdr.e_shstrndx * sizeof(elfShdr), SEEK_SET);
			fread(&elfShdr, 1, sizeof(elfShdr), fp);
			
			string_table = (char*)malloc(elfShdr.sh_size);
			fseek(fp, elfShdr.sh_offset, SEEK_SET);
			fread(string_table, 1, elfShdr.sh_size, fp);

			printf("[%-3s] %-16s %-18s %-18s %-10s\n", "NUM", "Name", "Type", "Address", "offset");
			printf("      %-18s %-18s %7s %-6s %-6s %-6s\n", "Size", "EntSize", "Flags", "Link", "Info", "Align");
			// read all section headers and print it
			for(i=0; i<elfEhdr.e_shnum; i++){
				fseek(fp, elfEhdr.e_shoff + i*elfEhdr.e_shentsize, SEEK_SET);
				fread(&elfShdr, 1, elfEhdr.e_shentsize, fp);

				printf("[%-3d]", i);
				printf("%-16s ", (string_table+elfShdr.sh_name));
				memset(section_type, 0, sizeof(section_type));
				switch(elfShdr.sh_type){
					case SHT_NULL:
						strcpy(section_type, "NULL");break;
					case SHT_PROGBITS:
						strcpy(section_type, "PROGBITS");break;
					case SHT_SYMTAB:
						strcpy(section_type, "SYMTAB");break;
					case SHT_STRTAB:
						strcpy(section_type, "STRTAB");break;
					case SHT_RELA:
						strcpy(section_type, "RELA");break;
					case SHT_HASH:
						strcpy(section_type, "HASH");break;
					case SHT_DYNAMIC:
						strcpy(section_type, "DYNAMIC");break;
					case SHT_NOTE:
						strcpy(section_type, "NOTE");break;
					case SHT_NOBITS:
						strcpy(section_type, "NOBITS");break;
					case SHT_REL:
						strcpy(section_type, "REL");break;
					case SHT_SHLIB:
						strcpy(section_type, "SHLIB");break;
					case SHT_DYNSYM:
						strcpy(section_type, "DYNSYM");break;
					case SHT_INIT_ARRAY:
						strcpy(section_type, "INIT_ARRAY");break;
					case SHT_FINI_ARRAY:
						strcpy(section_type, "FINI_ARRAY");break;
					case SHT_PREINIT_ARRAY:
						strcpy(section_type, "PREINIT_ARRAY");break;
					case SHT_GROUP:
						strcpy(section_type, "GROUP");break;
					case SHT_SYMTAB_SHNDX:
						strcpy(section_type, "SYMTAB_SHNDX");break;
					case SHT_LOOS:
						strcpy(section_type, "LOOS");break;
					case SHT_HIOS:
						strcpy(section_type, "HIOS");break;
					case SHT_LOPROC:
						strcpy(section_type, "LOPROC");break;
					case SHT_HIPROC:
						strcpy(section_type, "HIPROC");break;
					case SHT_LOUSER:
						strcpy(section_type, "LOUSER");break;
					case SHT_HIUSER:
						strcpy(section_type, "HIUSER");break;
				}
				printf("%-18s ", section_type);
				printf("0x%016u ", elfShdr.sh_addr);
				printf("0x%08u ", elfShdr.sh_offset);
				printf("\n      ");

				printf("0x%016u ", elfShdr.sh_size);
				printf("0x%016u ", elfShdr.sh_entsize);
				printf("%7u ", elfShdr.sh_flags);
				printf("%-6x ", elfShdr.sh_link);
				printf("%-6x ", elfShdr.sh_info);
				printf("%-6u ", elfShdr.sh_addralign);
				printf("\n");
				printf("\n");
			}
		}
		if(opt & OPT_PRO_HEADER){
			char program_type[16];

			fseek(fp, elfEhdr.e_phoff, SEEK_SET);
			printf("%-16s %-18s %-18s %-18s\n", "Type", "Offset", "VirtualAddr", "PhysicalAddr");
			printf("%-18s %-18s %-8s %7s\n", "FileSize", "MemorySize", "Flags", "Align" );
			for(i=0; i<elfEhdr.e_phnum; i++){
				fread(&elfPhdr, 1, elfEhdr.e_phentsize, fp);
				memset(program_type, 0, sizeof(program_type));
				switch(elfPhdr.p_type){
					case PT_NULL:
						strcpy(program_type, "NULL");break;
					case PT_LOAD:
						strcpy(program_type, "LOAD");break;
					case PT_DYNAMIC:
						strcpy(program_type, "DYNAMIC");break;
					case PT_INTERP:
						strcpy(program_type, "INTERP");break;
					case PT_NOTE:
						strcpy(program_type, "NOTE");break;
					case PT_SHLIB:
						strcpy(program_type, "SHLIB");break;
					case PT_PHDR:
						strcpy(program_type, "PHDR");break;
					case PT_TLS:
						strcpy(program_type, "TLS");break;
					case PT_LOOS:
						strcpy(program_type, "LOOS");break;
					case PT_HIOS:
						strcpy(program_type, "HIOS");break;
					case PT_LOPROC:
						strcpy(program_type, "LOPROC");break;
					case PT_HIPROC:
						strcpy(program_type, "HIPROC");break;
				}
				printf("%-16s ", program_type);
				printf("0x%016x ", elfPhdr.p_offset);
				printf("0x%016x ", elfPhdr.p_vaddr);
				printf("0x%016x ", elfPhdr.p_paddr);
				printf("\n");

				printf("0x%016x ", elfPhdr.p_filesz);
				printf("0x%016x ", elfPhdr.p_memsz);
				printf("%-8x ", elfPhdr.p_flags);
				printf("%7x ", elfPhdr.p_align);
				printf("\n\n");
			}

		}
	}
	else if(magic[EI_CLASS] == ELFCLASS64){
		Elf64_Ehdr elfEhdr;
		Elf64_Shdr elfShdr;
		Elf64_Phdr elfPhdr;

		fread(&elfEhdr, 1, sizeof(elfEhdr), fp);	// read elf file header
		if(opt == 0 || (opt & OPT_ELF_HEADER)){
			printf("ELF Header:\n");
			printf(" Magic: ");						// printf magic number
			for(i=0; i<EI_NIDENT; i++)
				printf("%02x ", elfEhdr.e_ident[i]);
			printf("\n");
			
			// analyze magic code
			printf(" Class: 64-bit objects\n");		// Class always is 64-bit objects
			printf(" Data: ");						
			switch(elfEhdr.e_ident[EI_DATA]){
				case ELFDATANONE:
					printf("Invalid data encoding"); break;
				case ELFDATA2LSB:
					printf("2's complement, little endian"); break;
				case ELFDATA2MSB:
					printf("2's complement, big endian"); break;
			}
			printf("\n");
			printf(" Version: ");
			switch(elfEhdr.e_ident[EI_VERSION]){
				case EV_NONE:
					printf("Invalid version");break;
				case EV_CURRENT:
					printf("Current version");break;
			}
			printf("\n");
			printf(" OS/ABI: ");
			switch(elfEhdr.e_ident[EI_OSABI]){
				case ELFOSABI_NONE:
					printf("No extension or unspecified");break;
				case ELFOSABI_HPUX:
					printf("Hewlett-Packard HP-UX");break;
					case ELFOSABI_NETBSD:
					printf("NetBSD");break;
				//case ELFOSABI_GNU:		//this value is same with ELFOSABI_LINUX
				//	printf("GNU");break;
				case ELFOSABI_LINUX:
					printf("Linux");break;
				case ELFOSABI_SOLARIS:
					printf("Sun Solaris");break;
				case ELFOSABI_AIX:
					printf("AIX");break;
				case ELFOSABI_IRIX:
					printf("IRIX");break;
				case ELFOSABI_FREEBSD:
					printf("FreeBSD");break;
				case ELFOSABI_TRU64:
					printf("Compaq TRU64 UNIX");break;
				case ELFOSABI_MODESTO:
					printf("Novell Modesto");break;
				case ELFOSABI_OPENBSD:
					printf("Open BSD");break;
				case ELFOSABI_OPENVMS:
					printf("Open VMS");break;
				case ELFOSABI_NSK:
					printf("Hewlett-Packard Non-Stop Kernel");break;
				case ELFOSABI_AROS:
					printf("Amiga Research OS");break;
				case ELFOSABI_FENIXOS:
					printf("The FenixOS highly scalable multi-core OS");break;
				case ELFOSABI_CLOUDABI:
					printf("Nuxi CloudABI");break;
				case ELFOSABI_OPENVOS:
					printf("Stratus Technologies OpenVOS");break;
			}
			printf("\n");
			printf(" ABI Version: ");
			switch(elfEhdr.e_ident[EI_ABIVERSION]){
				case EV_NONE:
					printf("invalid version");break;
				case EV_CURRENT:
					printf("Current version");break;
			}
			printf("\n");
			printf(" Type: ");
			switch(elfEhdr.e_type){
				case ET_NONE:
					printf("No file type");break;
				case ET_REL:
					printf("Relocatable object file");break;
				case ET_EXEC:
					printf("Executable file");break;
				case ET_DYN:
					printf("Shared object file");break;
				case ET_CORE:
					printf("Core file");break;
				case ET_LOOS:
				case ET_HIOS:
					printf("Environment_specific use");break;
				case ET_LOPROC:
				case ET_HIPROC:
					printf("Processor-specific use");break;
			}
			printf("\n");
			printf(" Machine: ");
			switch(elfEhdr.e_machine){
				case EM_NONE:
					printf("No machine");break;
				case EM_M32:
					printf("AT&T WE 32100");break;
				case EM_SPARC:
					printf("SPARC");break;
				case EM_386:
					printf("Intel 80386");break;
				case EM_68K:
					printf("Motorola 68000");break;
				case EM_88K:
					printf("Motorola 88000");break;
				case EM_IAMCU:
					printf("Intel MCU"); break;
				case EM_860:
					printf("Intel 80860");break;
				case EM_MIPS:
					printf("MIPS I Architecture");break;
				case EM_S370:
					printf("IBM System/370 Processor");break;
				case EM_MIPS_RS3_LE:
					printf("MIPS RS3000 Little-endian");break;
				case EM_PARISC:
					printf("Hewlett-Packard PA-RISC");break;
				case EM_VPP500:
					printf("Fujitsu VPP500");break;
				case EM_SPARC32PLUS:
					printf("Enhanced instruction set SPARC");break;
				case EM_960:
					printf("Intel 80960");break;
				case EM_PPC:
					printf("Power PC");break;
				case EM_PPC64:
					printf("64-bit Power PC");break;
				case EM_S390:
					printf("IBM System/390 Processor");break;
				case EM_SPU:
					printf("IBM SPU/SPC");break;
				case EM_V800:
					printf("NEC V800");break;
				case EM_FR20:
					printf("Fujitsu FR20");break;
				case EM_RH32:
					printf("TRW RH-32");break;
				case EM_RCE:
					printf("Motorola RCE");break;
				case EM_ARM:
					printf("ARM 32-bit architecture(AARCH32)");break;
				case EM_ALPHA:
					printf("Digital Alpha");break;
				case EM_SH:
					printf("Hitachi SH"); break;
				case EM_SPARCV9:
					printf("SPARC Version 9"); break;
				case EM_TRICORE:
					printf("Siemens TriCore embedded processor");break;
				case EM_ARC:
					printf("Argonaut RISC Core, Argonaut Technologies Inc.");break;
				case EM_H8_300:
					printf("Hitachi H8/300"); break;
				case EM_H8_300H:
					printf("Hitachi H8/300H");break;
				case EM_H8S:
					printf("Hitachi H8S");break;
				case EM_H8_500:
					printf("Hitachi H8/500");break;
				case EM_IA_64:
					printf("Intel IA-64 Processor architecture");break;
				case EM_MIPS_X:
					printf("Stanford MIPS-X");break;
				case EM_COLDFIRE:
					printf("Motorola ColdFire");break;
				case EM_68HC12:
					printf("Motorola M68HC12");break;
				case EM_MMA:
					printf("Fujitus MMA Multimedia Accelerator");break;
				case EM_PCP:
					printf("Siemens PCP");break;
				case EM_NCPU:
					printf("Sony nCPU embedded RISC Processor");break;
				case EM_NDR1:
					printf("Denso NDR1 microprocessor");break;
				case EM_STARCORE:
					printf("Motorola Star*Core Processor");break;
				case EM_ME16:
					printf("Toyota ME16 Processor");break;
				case EM_ST100:
					printf("STMicroelectronics ST100 processor");break;
				case EM_TINYJ:
					printf("Advanced Logic Corp. TinyJ embedded processor famlity");break;
				case EM_X86_64:
					printf("AMD x86-64 architecture");break;
				case EM_PDSP:
					printf("Sony DSP Processor");break;
				case EM_PDP10:
					printf("Digital Equipment Corp.PDP-10");break;
				case EM_PDP11:
					printf("Digital Equipment Corp.PDP-11");break;
				case EM_FX66:
					printf("Siemens FX66 microcontorller");break;
				case EM_ST9PLUS:
					printf("STMicroelectronics ST9+ 8/16 bit microcontroller");break;
				case EM_ST7:
					printf("StMicroelectronics ST7 6-bit microcontroller"); break;
				case EM_68HC16:
					printf("Motorola MC68HC16 Microcontroller");break;
				case EM_68HC11:
					printf("Motorola MC68HC11 Microcontroller");break;
				case EM_68HC08:
					printf("Motorola MC68HC08 Microcontroller");break;
				case EM_68HC05:
					printf("Motorola MC68HC05 Microcontroller");break;
				case EM_SVX:
					printf("Silicon Graphics SVx");break;
				case EM_ST19:
					printf("STMicroelectronics ST19 8-bit microcontroller");break;
				case EM_VAX:
					printf("Digital VAX");break;
				case EM_CRIS:
					printf("Axis Communication 32-bit embedded processor");break;
				case EM_JAVELIN:
					printf("Infineon Technologies 32-bit embedded processor");break;
				case EM_FIREPATH:
					printf("Element 14 64-bit DSP processor");break;
				case EM_ZSP:
					printf("LSI Logic 16-bit DSP Processor");break;
				case EM_MMIX:
					printf("Donald Knuth's educational 64-bit processor");break;
				case EM_HUANY:
					printf("Harvard University machine-independent object file");break;
				case EM_PRISM:
					printf("Sitera Prism");break;
				case EM_AVR:
					printf("Atmel AVR 8-bit microcontroller");break;
				case EM_FR30:
					printf("Fujitus FR30");break;
				case EM_D10V:
					printf("Mitsubishi D10V");break;
				case EM_D30V:
					printf("Mitsubishi D30V");break;
				case EM_V850:
					printf("NEC v850");break;
				case EM_M32R:
					printf("Mitsubishi M32R");break;
				case EM_MN10300:
					printf("Matsushita MN10300");break;
				case EM_MN10200:
					printf("Matsushita MN10200");break;
				case EM_PJ:
					printf("picoJava");break;
				case EM_OPENRISC:
					printf("OpenRISC 32-bit embedded processor");break;
				case EM_ARC_COMPACT:
					printf("ARC International ARCompact processor (old spelling/synonym: EM_ARC_A5)");break;
				case EM_XTENSA:
					printf("Tensilica Xtensa Architecture");break;
				case EM_VIDEOCORE:
					printf("Alphamosaic VideoCore Processor");break;
				case EM_TMM_GPP:
					printf("Thompson Multimedia General Purpose Processor");break;
				case EM_NS32K:
					printf("National Semiconductor 32000 series");break;
				case EM_TPC:
					printf("Tenor Network TPC processor");break;
				case EM_SNP1K:
					printf("Trebia SNP 1000 Processor");break;
				case EM_ST200:
					printf("STMicroelectronics (www.st.com) ST200 microcontroller");break;
				case EM_IP2K:
					printf("Ubicom IP2xxx microcontroller family");break;
				case EM_MAX:
					printf("MAX Processor");break;
				case EM_CR:
					printf("National Semiconductor CompactRISC microprocessor");break;
				case EM_F2MC16:
					printf("Fujitsu F2MC16");break;
				case EM_MSP430:
					printf("Texas Instrucments embedded microcontroller msp430");break;
				case EM_BLACKFIN:
					printf("Analog Devices Blackfin(DSP) processor");break;
				case EM_SE_C33:
					printf("S1C33 Family of Seiko Epson processor");break;
				case EM_SEP:
					printf("Sharp embedded microprocessor");break;
				case EM_ARCA:
					printf("Arca RISC microprocessor");break;
				case EM_UNICORE:
					printf("Microprocessor series from PKU_Unity Ltd. and MPRC of Peking University");break;
				case EM_EXCESS:
					printf("eXcess: 16/32/64-bit configurable embedded CPU");break;
				case EM_DXP:
					printf("Icera Semiconductor Inc. Deep Execution Processor");break;
				case EM_ALTERA_NIOS2:
					printf("Altera Nios 2 soft-core processor");break;
				case EM_CRX:
					printf("National Semiconductor CompactRISC CRX microprocessor");break;
				case EM_XGATE:
					printf("Motorola XGATE embedded processor");break;
				case EM_C166:
					printf("Infineon C16x/XC16x Processor");break;
				case EM_M16C:
					printf("Renesas M16C series microprocessors");break;
				case EM_DSPIC30F:
					printf("Microchip Technology dsPIC30F DIgital Signal Controller");break;
				case EM_CE:
					printf("Freescale Communication Engine RISC core");break;
				case EM_TSK3000:
					printf("Altium TSK3000 core");break;
				case EM_RS08:
					printf("Fresscale RS08 embedded processor");break;
				case EM_SHARC:
					printf("Analog Devices SHARC family of 32-bit DSP processors");break;
				case EM_ECOG2:
					printf("Cyan Tchnology eCOG2 microprocessor");break;
				case EM_SCORE7:
					printf("Sunplus S+core7 RISC processor");break;
				case EM_DSP24:
					printf("New Japan Radio(NJR) 24-bit DSP Processor");break;
				case EM_VIDEOCORE3:
					printf("Broadcom VeideoCore3 Processor");break;
				case EM_LATTICEMICO32:
					printf("RISC processor for Lattice FPGA architecture");break;
				case EM_SE_C17:
					printf("Seiko Epson C17 Family");break;
				case EM_TI_C6000:
					printf("The Texas Instruments TMS320C6000 DSP family");break;
				case EM_TI_C2000:
					printf("The Texas Instruments TMS320C2000 DSP family");break;
				case EM_TI_C5500:
					printf("The Texas Instruments TMS320C55x DSP family");break;
				case EM_TI_ARP32:
					printf("Texas Instruments Application Specific RISC Processor, 32bit fetch");break;
				case EM_TI_PRU:
					printf("Texas Instruments Programmable Runtime Unit");break;
				case EM_MMDSP_PLUS:
					printf("STMicroelectronics 64bits VLIW Data Signal Processor");break;
				case EM_CYPRESS_M8C:
					printf("Cypress M8C microprocessor");break;
				case EM_R32C:
					printf("Renesas R32C series microprocessors");break;
				case EM_TRIMEDIA:
					printf("NXP Semiconductors TriMedia architecture family");break;
				case EM_QDSP6:
					printf("QUALCOMM DSP6 Processor");break;
				case EM_8051:
					printf("Intel 8051 and variants");break;
				case EM_STXP7X:
					printf("STMicroelectronics STxP7x family of configurable and exensible RISC processors");break;
				case EM_NDS32:
					printf("Andes Technology compact code size embedded RISC processor family");break;
				case EM_ECOG1: // == case EM_ECOG1X
					printf("Cyan Technology eCOG1X family");break;
				case EM_MAXQ30:
					printf("Dallas Semiconductor MAXQ30 Core Micro-controllers");break;
				case EM_XIMO16:
					printf("New Japan Radio (NJR) 16-bit DSP Processor");break;
				case EM_MANIK:
					printf("M2000 REconfigutable RISC Microprocessor");break;
				case EM_CRAYNV2:
					printf("Cray Inc. NV2 vector Architecture");break;
				case EM_RX:
					printf("Renesas RX family");break;
				case EM_METAG:
					printf("Imagination Technologies META processor architecture");break;
				case EM_MCST_ELBRUS:
					printf("MCST Elbrus general purpose hardware architecture");break;
				case EM_ECOG16:
					printf("Cyan Technology eCOG16 family");break;
				case EM_CR16:
					printf("National Semiconductor CompactRISC CR16 16-bit microprocessor");break;
				case EM_ETPU:
					printf("Freescale Extended Time processing Unit");break;
				case EM_SLE9X:
					printf("Infineon TEchnologies SLE9X core");break;
				case EM_L10M:
					printf("Intel L10M");break;
				case EM_K10M:
					printf("Intel K10M");break;
				case EM_AARCH64:
					printf("ARM 64-bit architecture (AARCH64)");break;
				case EM_AVR32:
					printf("Atmel Corporation 32-bit microprocessor family");break;
				case EM_STM8:
					printf("STMicroelectronics STM8 8-bit microcontroller");break;
				case EM_TILE64:
					printf("Tilera TILE64 multicore acrchitecture family");break;
				case EM_TILEPRO:
					printf("Tilera TILEPro multicore architecture family");break;
				case EM_MICROBLAZE:
					printf("Xilinx MicroBlaze 32-bit RISC soft processor core");break;
				case EM_CUDA:
					printf("NVIDIA CUDA Architecture");break;
				case EM_TILEGX:
					printf("Tilera TILE-Gx multicore architecture family");break;
				case EM_CLOUDSHIELD:
					printf("CloudShield architecture family");break;
				case EM_COREA_1ST:
					printf("KIPO-KAIST Core-A 1st generation processor family");break;
				case EM_COREA_2ND:
					printf("KIPO-KAIST Core-A 2nd generation processor family");break;
				case EM_ARC_COMPACT2:
					printf("Synopsys ArCompcat V2");break;
				case EM_OPEN8:
					printf("Open8 8-bit RISC soft processor core");break;
				case EM_RL78:
					printf("Renesas RL78 family");break;
				case EM_VIDEOCORE5:
					printf("Broadcom VideoCore V processor");break;
				case EM_78KOR:
					printf("Renesas 78KOR family");break;
				case EM_56800EX:
					printf("Freescale 56800EX Digital Signal Controller (DSC)");break;
				case EM_BA1:
					printf("Beyond BA1 CPU Architecture");break;
				case EM_BA2:
					printf("Beyond BA2 CPU Architecture");break;
				case EM_XCORE:
					printf("XMOS xCORE processor family");break;
				case EM_MCHP_PIC:
					printf("Microchip 8-bit PIC(r) family");break;
				case EM_INTEL205:
				case EM_INTEL206:
				case EM_INTEL207:
				case EM_INTEL208:
				case EM_INTEL209:
					printf("Reserved by Intel");break;
				case EM_KM32:
					printf("KM211 KM32 32-bit processor");break;
				case EM_KMX32:
					printf("KM211 KMX32 32-bit processor");break;
				case EM_KMX16:
					printf("KM211 KMX16 16-bit processor");break;
				case EM_KMX8:
					printf("KM211 KMX8 8-bit processor");break;
				case EM_KVARC:
					printf("KM211 KVARC processor");break;
				case EM_CDP:
					printf("Paneve CDP architecture family");break;
				case EM_COGE:
					printf("Cognitive Smart Memory Processor");break;
				case EM_COOL:
					printf("Bluechip Systems CoolEngine");break;
				case EM_NORC:
					printf("Nanoradio OPtimized RISC");break;
				case EM_CSR_KALIMBA:
					printf("CSR Kalimba architecture family");break;
				case EM_Z80:
					printf("Zilog Z80");break;
				case EM_VISIUM:
					printf("Controls and Data services VISIUMcore processor");break;
				case EM_FT32:
					printf("FTDI chip FT32 high performance 32-bit RISC architecture");break;
				case EM_MOXIE:
					printf("Moxie processor family");break;
				case EM_AMDGPU:
					printf("AMD GPU architecture");break;
				case EM_RISCV:
					printf("RISC-V");break;
			}
			printf("\n");
			printf(" Version: ");
			switch(elfEhdr.e_version){
				case EV_NONE:
					printf("Invalid version");break;
				case EV_CURRENT:
					printf("Current version");break;
			}
			printf("\n");
			printf(" Etnry point address: 0x%llx\n", elfEhdr.e_entry);
			printf(" Start of program header: %lld (bytes into file)\n", elfEhdr.e_phoff);
			printf(" Start of Section header: %lld (bytes into file)\n", elfEhdr.e_shoff);
			printf(" Flags: 0x%x\n", elfEhdr.e_flags);
			printf(" Size of this header: %d (btyes)\n", elfEhdr.e_ehsize);
			printf(" Size of program headers: %d (bytes)\n", elfEhdr.e_phentsize);
			printf(" Number of program headers: %d\n", elfEhdr.e_phnum);
			printf(" Size of Section headers: %d (bytes)\n", elfEhdr.e_shentsize);
			printf(" Number of Section headers: %d\n", elfEhdr.e_shnum);
			printf(" Section header string table index: %d\n", elfEhdr.e_shstrndx);
			printf("\n");
		}
		if(opt & OPT_SEC_HEADER){
			char *string_table;
			char section_type[16];
			// read string table
			fseek(fp, elfEhdr.e_shoff + elfEhdr.e_shstrndx * sizeof(elfShdr), SEEK_SET);
			fread(&elfShdr, 1, sizeof(elfShdr), fp);
			
			string_table = (char*)malloc(elfShdr.sh_size);
			fseek(fp, elfShdr.sh_offset, SEEK_SET);
			fread(string_table, 1, elfShdr.sh_size, fp);

			printf("[%-3s] %-16s %-18s %-18s %-10s\n", "NUM", "Name", "Type", "Address", "offset");
			printf("      %-18s %-18s %7s %-6s %-6s %-6s\n", "Size", "EntSize", "Flags", "Link", "Info", "Align");
			// read all section headers and print it
			for(i=0; i<elfEhdr.e_shnum; i++){
				fseek(fp, elfEhdr.e_shoff + i*elfEhdr.e_shentsize, SEEK_SET);
				fread(&elfShdr, 1, elfEhdr.e_shentsize, fp);

				printf("[%-3d]", i);
				printf("%-16s ", (string_table+elfShdr.sh_name));
				memset(section_type, 0, sizeof(section_type));
				switch(elfShdr.sh_type){
					case SHT_NULL:
						strcpy(section_type, "NULL");break;
					case SHT_PROGBITS:
						strcpy(section_type, "PROGBITS");break;
					case SHT_SYMTAB:
						strcpy(section_type, "SYMTAB");break;
					case SHT_STRTAB:
						strcpy(section_type, "STRTAB");break;
					case SHT_RELA:
						strcpy(section_type, "RELA");break;
					case SHT_HASH:
						strcpy(section_type, "HASH");break;
					case SHT_DYNAMIC:
						strcpy(section_type, "DYNAMIC");break;
					case SHT_NOTE:
						strcpy(section_type, "NOTE");break;
					case SHT_NOBITS:
						strcpy(section_type, "NOBITS");break;
					case SHT_REL:
						strcpy(section_type, "REL");break;
					case SHT_SHLIB:
						strcpy(section_type, "SHLIB");break;
					case SHT_DYNSYM:
						strcpy(section_type, "DYNSYM");break;
					case SHT_INIT_ARRAY:
						strcpy(section_type, "INIT_ARRAY");break;
					case SHT_FINI_ARRAY:
						strcpy(section_type, "FINI_ARRAY");break;
					case SHT_PREINIT_ARRAY:
						strcpy(section_type, "PREINIT_ARRAY");break;
					case SHT_GROUP:
						strcpy(section_type, "GROUP");break;
					case SHT_SYMTAB_SHNDX:
						strcpy(section_type, "SYMTAB_SHNDX");break;
					case SHT_LOOS:
						strcpy(section_type, "LOOS");break;
					case SHT_HIOS:
						strcpy(section_type, "HIOS");break;
					case SHT_LOPROC:
						strcpy(section_type, "LOPROC");break;
					case SHT_HIPROC:
						strcpy(section_type, "HIPROC");break;
					case SHT_LOUSER:
						strcpy(section_type, "LOUSER");break;
					case SHT_HIUSER:
						strcpy(section_type, "HIUSER");break;
				}
				printf("%-18s ", section_type);
				printf("0x%016llu ", elfShdr.sh_addr);
				printf("0x%08llu ", elfShdr.sh_offset);
				printf("\n      ");

				printf("0x%016llu ", elfShdr.sh_size);
				printf("0x%016llu ", elfShdr.sh_entsize);
				printf("%7llu ", elfShdr.sh_flags);
				printf("%-6x ", elfShdr.sh_link);
				printf("%-6x ", elfShdr.sh_info);
				printf("%-6llu ", elfShdr.sh_addralign);
				printf("\n");
				printf("\n");
			}
		}
		if(opt & OPT_PRO_HEADER){
			char program_type[16];

			fseek(fp, elfEhdr.e_phoff, SEEK_SET);
			printf("%-16s %-18s %-18s %-18s\n", "Type", "Offset", "VirtualAddr", "PhysicalAddr");
			printf("%-18s %-18s %-8s %7s\n", "FileSize", "MemorySize", "Flags", "Align" );
			for(i=0; i<elfEhdr.e_phnum; i++){
				fread(&elfPhdr, 1, elfEhdr.e_phentsize, fp);
				memset(program_type, 0, sizeof(program_type));
				switch(elfPhdr.p_type){
					case PT_NULL:
						strcpy(program_type, "NULL");break;
					case PT_LOAD:
						strcpy(program_type, "LOAD");break;
					case PT_DYNAMIC:
						strcpy(program_type, "DYNAMIC");break;
					case PT_INTERP:
						strcpy(program_type, "INTERP");break;
					case PT_NOTE:
						strcpy(program_type, "NOTE");break;
					case PT_SHLIB:
						strcpy(program_type, "SHLIB");break;
					case PT_PHDR:
						strcpy(program_type, "PHDR");break;
					case PT_TLS:
						strcpy(program_type, "TLS");break;
					case PT_LOOS:
						strcpy(program_type, "LOOS");break;
					case PT_HIOS:
						strcpy(program_type, "HIOS");break;
					case PT_LOPROC:
						strcpy(program_type, "LOPROC");break;
					case PT_HIPROC:
						strcpy(program_type, "HIPROC");break;
				}
				printf("%-16s ", program_type);
				printf("0x%016llx ", elfPhdr.p_offset);
				printf("0x%016llx ", elfPhdr.p_vaddr);
				printf("0x%016llx ", elfPhdr.p_paddr);
				printf("\n");

				printf("0x%016llx ", elfPhdr.p_filesz);
				printf("0x%016llx ", elfPhdr.p_memsz);
				printf("%-8x ", elfPhdr.p_flags);
				printf("%7llx ", elfPhdr.p_align);
				printf("\n\n");
			}

		}
	}
	else if(magic[EI_CLASS] == ELFCLASSNONE){
		printf("Invalid Class\n");
	}
	fclose(fp);
	return 0;
}

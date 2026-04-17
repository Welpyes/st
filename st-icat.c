#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/stat.h>
#include <dirent.h>

static const char *diacritics[] = {
	"̅", "̍", "̎", "̐", "̒", "̽", "̾", "̿", "͆", "͊", "͋", "͌", "͐", "͑", "͒", "͗", "͛", "ͣ", "ͤ", "ͥ", "ͦ", "ͧ", "ͨ", "ͩ", "ͪ", "ͫ", "ͬ", "ͭ", "ͮ", "ͯ", "҃", "҄", "҅", "҆", "҇", "֒", "֓", "֔", "֕", "֗", "֘", "֙", "֜", "֝", "֞", "֟", "֠", "֡", "֨", "֩", "֫", "֬", "֯", "ׄ", "ؐ", "ؑ", "ؒ", "ؓ", "ؔ", "ؕ", "ؖ", "ؗ", "ٗ", "٘", "ٙ", "ٚ", "ٛ", "ٝ", "ٞ", "ۖ", "ۗ", "ۘ", "ۙ", "ۚ", "ۛ", "ۜ", "۟", "۠", "ۡ", "ۢ", "ۤ", "ۧ", "ۨ", "۫", "۬", "ܰ", "ܲ", "ܳ", "ܵ", "ܶ", "ܺ", "ܽ", "ܿ", "݀", "݁", "݃", "݅", "݇", "݉", "݊", "߫", "߬", "߭", "߮", "߯", "߰", "߱", "߳", "ࠖ", "ࠗ", "࠘", "࠙", "ࠛ", "ࠜ", "ࠝ", "ࠞ", "ࠟ", "ࠠ", "ࠡ", "ࠢ", "ࠣ", "ࠥ", "ࠦ", "ࠧ", "ࠩ", "ࠪ", "ࠫ", "ࠬ", "࠭", "॑", "॓", "॔", "ྂ", "ྃ", "྆", "྇", "፝", "፞", "፟", "៝", "᤺", "ᨗ", "᩵", "᩶", "᩷", "᩸", "᩹", "᩺", "᩻", "᩼", "᭫", "᭭", "᭮", "᭯", "᭰", "᭱", "᭲", "᭳", "᳐", "᳑", "᳒", "᳚", "᳛", "᳠", "᷀", "᷁", "᷃", "᷄", "᷅", "᷆", "᷇", "᷈", "᷉", "᷋", "᷌", "᷑", "᷒", "ᷓ", "ᷔ", "ᷕ", "ᷖ", "ᷗ", "ᷘ", "ᷙ", "ᷚ", "ᷛ", "ᷜ", "ᷝ", "ᷞ", "ᷟ", "ᷠ", "ᷡ", "ᷢ", "ᷣ", "ᷤ", "ᷥ", "ᷦ", "᷾", "⃐", "⃑", "⃔", "⃕", "⃖", "⃗", "⃛", "⃜", "⃡", "⃧", "⃩", "⃰", "⳯", "⳰", "⳱", "ⷠ", "ⷡ", "ⷢ", "ⷣ", "ⷤ", "ⷥ", "ⷦ", "ⷧ", "ⷨ", "ⷩ", "ⷪ", "ⷫ", "ⷬ", "ⷭ", "ⷮ", "ⷯ", "ⷰ", "ⷱ", "ⷲ", "ⷳ", "ⷴ", "ⷵ", "ⷶ", "ⷷ", "ⷸ", "ⷹ", "ⷺ", "ⷻ", "ⷼ", "ⷽ", "ⷾ", "ⷿ", "꙯", "꙼", "꙽", "꛰", "꛱", "꣠", "꣡", "꣢", "꣣", "꣤", "꣥", "꣦", "꣧", "꣨", "꣩", "꣪", "꣫", "꣬", "꣭", "꣮", "꣯", "꣰", "꣱", "ꪰ", "ꪲ", "ꪳ", "ꪷ", "ꪸ", "ꪾ", "꪿", "꫁", "︠", "︡", "︢", "︣", "︤", "︥", "︦", "𐨏", "𐨸", "𝆅", "𝆆", "𝆇", "𝆈", "𝆉", "𝆪", "𝆫", "𝆬", "𝆭", "𝉂", "𝉃", "𝉄"
};

static char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length) {
	static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	*output_length = 4 * ((input_length + 2) / 3);
	char *encoded_data = malloc(*output_length + 1);
	if (!encoded_data) return NULL;
	for (int i = 0, j = 0; i < input_length;) {
		uint32_t octet_a = i < input_length ? data[i++] : 0;
		uint32_t octet_b = i < input_length ? data[i++] : 0;
		uint32_t octet_c = i < input_length ? data[i++] : 0;
		uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
		encoded_data[j++] = table[(triple >> 3 * 6) & 0x3F];
		encoded_data[j++] = table[(triple >> 2 * 6) & 0x3F];
		encoded_data[j++] = table[(triple >> 1 * 6) & 0x3F];
		encoded_data[j++] = table[(triple >> 0 * 6) & 0x3F];
	}
	for (int i = 0; i < (3 - input_length % 3) % 3; i++)
		encoded_data[*output_length - 1 - i] = '=';
	encoded_data[*output_length] = '\0';
	return encoded_data;
}

void transmit_file(const char *filename, unsigned int id, int cols, int rows, int is_first, int delay_ms) {
	FILE *f = fopen(filename, "rb");
	if (!f) return;
	fseek(f, 0, SEEK_END);
	size_t fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	unsigned char *fdata = malloc(fsize);
	fread(fdata, 1, fsize, f);
	fclose(f);

	size_t b64size;
	char *b64data = base64_encode(fdata, fsize, &b64size);
	free(fdata);

	if (is_first)
		printf("\033_Gq=2,a=T,U=1,i=%u,f=100,c=%d,r=%d,m=1;", id, cols, rows);
	else
		printf("\033_Gq=2,a=f,i=%u,f=100,z=%d,m=1;", id, delay_ms);

	size_t pos = 0, chunk_size = 4000;
	while (pos < b64size) {
		size_t len = b64size - pos;
		if (len > chunk_size) len = chunk_size;
		int more = (pos + len < b64size) ? 1 : 0;
		if (pos > 0) printf("\033_Gq=2,i=%u,m=%d;", id, more);
		fwrite(b64data + pos, 1, len, stdout);
		printf("\033\\");
		pos += len;
	}
	free(b64data);
}

int main(int argc, char *argv[]) {
	if (argc < 2) { fprintf(stderr, "Usage: %s <file>\n", argv[0]); return 1; }
	
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	int img_w = 800, img_h = 600, frames = 1;
	char cmd[2048];
	
	/* Get info */
	snprintf(cmd, sizeof(cmd), "magick identify -format '%%w %%h %%n' \"%s\" 2>/dev/null", argv[1]);
	FILE *p = popen(cmd, "r");
	if (p) { fscanf(p, "%d %d %d", &img_w, &img_h, &frames); pclose(p); }

	int cw = (ws.ws_xpixel > 0) ? ws.ws_xpixel / ws.ws_col : 10;
	int ch = (ws.ws_ypixel > 0) ? ws.ws_ypixel / ws.ws_row : 20;
	int cols = img_w / cw, rows = img_h / ch;
	if (cols < 1) cols = 1; if (rows < 1) rows = 1;
	if (cols > ws.ws_col) { rows = rows * ws.ws_col / cols; cols = ws.ws_col; }
	if (rows > ws.ws_row - 2) { cols = cols * (ws.ws_row - 2) / rows; rows = ws.ws_row - 2; }

	srand(getpid());
	unsigned int id = (rand() % 0xFFFFFF) + 1;
	while (((id / 256) % 65536) == 0) id++;

	if (frames > 1) {
		char tmpdir[512];
		snprintf(tmpdir, sizeof(tmpdir), "/data/data/com.termux/files/usr/tmp/st-icat-%d", getpid());
		mkdir(tmpdir, 0700);
		snprintf(cmd, sizeof(cmd), "ffmpeg -v quiet -i \"%s\" -vsync 0 \"%s/f%%05d.png\" 2>/dev/null", argv[1], tmpdir);
		system(cmd);

		/* Read per-frame delays (GIF stores in centiseconds) */
		int delays[4096]; int dcount = 0;
		{
			char dcmd[2048];
			snprintf(dcmd, sizeof(dcmd), "magick identify -format \'%%T\\n\' \"%s\" 2>/dev/null", argv[1]);
			FILE *dp = popen(dcmd, "r");
			if (dp) {
				int v;
				while (dcount < 4096 && fscanf(dp, "%d", &v) == 1)
					delays[dcount++] = (v < 2) ? 100 : v * 10;
				pclose(dp);
			}
		}

		/* Collect and sort frame paths */
		DIR *d = opendir(tmpdir);
		struct dirent *dir;
		char *fpaths[4096]; int fcount = 0;
		while ((dir = readdir(d)) != NULL)
			if (strstr(dir->d_name, ".png") && fcount < 4096) {
				char *fp = malloc(1024);
				snprintf(fp, 1024, "%s/%s", tmpdir, dir->d_name);
				fpaths[fcount++] = fp;
			}
		closedir(d);
		for (int i = 0; i < fcount-1; i++)
			for (int j = i+1; j < fcount; j++)
				if (strcmp(fpaths[i], fpaths[j]) > 0) { char *t = fpaths[i]; fpaths[i] = fpaths[j]; fpaths[j] = t; }

		for (int fi = 0; fi < fcount; fi++) {
			char *fpath = fpaths[fi];
			int delay_ms = (fi < dcount) ? delays[fi] : 100;
			transmit_file(fpath, id, cols, rows, fi == 0, delay_ms);
			if (fi == 0) {
				/* After first frame: set loading mode with frame 1 delay */
				printf("\033_Gq=2,a=a,v=1,s=2,r=1,z=%d,i=%u\033\\", delay_ms, id);
				/* Print placeholder grid */
				const char *ph = "\xf4\x8e\xbb\xae";
				unsigned int r = (id >> 16) & 0xFF, g = (id >> 8) & 0xFF, b = id & 0xFF, id4 = (id >> 24) & 0xFF;
				printf("\033[0m");
				for (int y = 0; y < rows; y++) {
					printf("\033[38;2;%u;%u;%um", r, g, b);
					for (int x = 0; x < cols; x++)
						printf("%s%s%s%s", ph, diacritics[y%297], diacritics[x%297], diacritics[id4%297]);
					printf("\033[39m\n");
				}
				printf("\033[0m");
				fflush(stdout);
			}
			unlink(fpath);
			free(fpath);
		}
		rmdir(tmpdir);
		/* Switch to loop mode after all frames uploaded */
		printf("\033_Gq=2,a=a,v=1,s=3,i=%u\033\\", id);
	} else {
		transmit_file(argv[1], id, cols, rows, 1, 0);
		const char *ph = "\xf4\x8e\xbb\xae";
		unsigned int r = (id >> 16) & 0xFF, g = (id >> 8) & 0xFF, b = id & 0xFF, id4 = (id >> 24) & 0xFF;
		printf("\033[0m");
		for (int y = 0; y < rows; y++) {
			printf("\033[38;2;%u;%u;%um", r, g, b);
			for (int x = 0; x < cols; x++)
				printf("%s%s%s%s", ph, diacritics[y%297], diacritics[x%297], diacritics[id4%297]);
			printf("\033[39m\n");
		}
		printf("\033[0m");
	}
	return 0;
}

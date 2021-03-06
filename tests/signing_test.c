#include "test_common.h"
#include "iota/conversion.h"
#include "iota/signing.h"
#include "iota/bundle.c"

#define MAX_SECURITY 3
#define MAX_SIGNATURE_LENGTH ((MAX_SECURITY)*27 * (NUM_HASH_TRYTES))

// for purely coincidental reasons this seed was intialy used in the development
static const char PETER_SEED[] =
    "PETERPETERPETERPETERPETERPETERPETERPETERPETERPETERPETERPETERPETERPETERPETE"
    "RPETERR";
static const uint32_t IDX = 0;

static const tryte_t NORMALIZED_HASH[NUM_HASH_TRYTES] = {0};

static const char *FRAGMENTS[] = {
    "GTYOKCTTBPHTOTHZPNCNVFSFXGPSPUYFPUMSJKGJIBLLRBLKBEQEVXQT9QZWIKYDTYHZU9YOGX"
    "IMIGHJWZICUMVD9NDTLK9UWTWYIMAYNN9V9VPVUQPXZMFUTQZCFUNMONHDQYYFKUMIPJTGCZNC"
    "MHKDLHNEYFVZGAUO9LSSZNXIYPTIXZQFZSYONIVMLWRZRQHPJTYALCBHPKNTD9SZTBNQMZF9VN"
    "UDMMAYVGJCMQA9TCGX9ZDVOOQKZPDLVUI9V9EBNWIEWMTOZZPMAEFB9MWRPAKIKGZQAVTCQMSF"
    "IQSQWNF9RHOLEILBIIBKUF9W9VLXCTESUSBZWOWLTZRDGYBVZUWEFSHBGOVKYXDNWQTISFDBCG"
    "KHVWNVNLMUVPWGTPROYXVEGIVQGTDMNGXFAYAWALMJG9HXBQVMCAQHDXV9FYIMMXWJNQFPAJAB"
    "RGSBCBLRKUW9TRNRPX9QYTDOOYELJVMYPFBCTUBXYWMHSHBJCKXGPDISTD9PFCPUDKJUBPJ9OV"
    "QAEVHNUOENWPCLTK9ELFHZNSEKHYXDPIXMPXUQSGFYMUYBD9CMEZEVIAJAOQY9BNWW9SSHQSPF"
    "MRDPGOTTHGJYJCWDGNGMOERGLWACCSRSVS9BPMDYDVLKRTEMVTYBVTZXYPJJWITXGCBAIVBPNP"
    "YPPMDFXISEVOOGXPDFPHHRDF9QPHMHKKCHOFSCHWSBVNURI9OMBDCWNGFLNSUZYLTCLQYRHDBG"
    "XK9QKQVNGDSEEDKRHKKJYMJHHKKCHTDZTLPZ99H9AFGFPHMNFX9IHCTCCEWAJRQKLNKDQASOZV"
    "DKLELBHKKLERVASSJJKFBOWSS9QHOKITYRNLVVTBFBTQMLACOQHZLIDXROFCGZQEZCDIIXVLYW"
    "AKANHWOAVXDYDVIUVAVPDQDGPTJEDZBHSVLPDACRYMBH9YTKXYDJJCPYYJ9YSRMTUCAGICHUWW"
    "ZNPFVAD9XYYJEFOBAHFUWZBWVJFJCOBJMPENYHYFPFCUNDNSOBROOLPKFKXJTGIXIEESNDNRJE"
    "HABFCMTCHPUUM9UVDKGGC9DVNGOFKDLZRL9FFTEIYZQXKHKSLD9H9NRKBGMABDNBQHYSMHQ9WP"
    "DTGV9WWPSNDEUWSFQGXSLITWWTYLYUXWFDBKETVEFVRVUGUVKKMPNQCE9BPMSIVQNKAEBRDABE"
    "CJPCGRWRISMUAAPYTEGIKALVXZFMGJCLECRAJBCEYKFLUSPGUIQXLRYABUHBEGWPRMBPLXQGON"
    "LMXIDZUB9NPDKGOHQNTVEUTYIWLBVLSYTSQFRDZODQOMAFSMXPIGKPYPQYHUBSQUUES9VFVSXO"
    "O9ATNXWOXOGCTADMQIORRFSKPAVHQP9KXULNFWIOBGNVYNQT9DHMBMLTYCJZOFDRRWNWZLHSTX"
    "9VCIPAPDXHXFXSWO9KSEDWCTNLFYSVYARMWGAUNHVGNJEMKUPOSXBPQQGYEPSBVYYOEWXPUXZS"
    "IJAOTYFPUMGSRMXIWFOHNSHCRWLGXXXRZOQSKYSMV9IRMYCKIARXDHLRVBDVIQZUGDBEVGMXXG"
    "RAOU9MMHYZYIWWHKAISKQMDWGVZ9DPQEUQEFRRNKYWIVRSWNFDICIGIWIANMEVIUDYPVVXOWXQ"
    "MFPXGB9ZUUQXZXWFFLBVPXPKFCCZIGFXWWCHOQKG9ISFND9BYPLWWKSXZXGXEOKUNMKKLVNLBZ"
    "ZLGSOZKLUXHMU9RVA9DWDVZORALEFEJBPHSTKFYKYOUUHGJCMTQP9QGDKYPYRGVBTBBTBANS9M"
    "LDCYBWQTKOJBRPYGLATNLPMTHMF9LMRWDRBDXIWALRDXENEBVVHURXDVJBKOXEAKIZNATPZZBQ"
    "WDTXLYTBCRRZZYYLWBRFG9TTOBGLRCQRGJDH9XINHSQMRGPCWRGDPZLWTEEB9RFHGVXEQHDWAH"
    "KLEOD9EBTSYBM9AKMI9DAJAKITFZYFTDMID99MDMFXBHLHOGMSAZFTELQEBSJPCLGPUHSZCDXK"
    "URSLVLAXNAUWRPGUBKMUTYEEHGZICNZEXZBDVINBMSM9EIDKVQXEECPITPQLSNUPAPAZ9DDVCN"
    "9RDUMQUNXE9IBBBDTX9EM9ALRXQEWMONECCTSPQKJ9FFIBXXN9WKFIDBRLOVJSHZEXBOJAGXLV"
    "HMFZWAVGWHCLQNSUSAOIMMBNQOA9YLZCZKWYPIPNZ",
    "TKNSJNOADOTWPGZXEVSFQSLF9DAVKMIS9FPRPBWMQXYWOLSDZFHGILPWTPAECIUEVHAHQUCHNI"
    "ZOECUUWCUQUCJKESYHQCVPXBXAEJVRZMNZJUMQISLBNDQXVPZKEQGNHSPKVWGQQFKGKJSYCKHV"
    "AYQSMDJDTTSSTADSAZTRZBQQMFJWIQNGRYMJLHV9QESLKEPZQXEHRNZXHYFEEMYDZXGYORZYIK"
    "SVGMYLP9BLGYGIEZKIK9WTXFPEOSQFASMH9LFXXQPGGAAXIANPPMAKBXWAUQNCDPDEIPHASEFN"
    "LQKYSQCBEKPODCXESKJ9TLJXUJNZMBEUUYYKBKZPNU9KOVKFJSYNEESFBUKIODZLUIOLIN9ZKB"
    "CGVRZQGCUHGRZAQUANSPX9PUHGHHDLPSJU9AGRBDZAMIRTOGRZNKLBIEMBISDRXBNB9ZDGA9KV"
    "IHGFHVGZHJRLNTUWCAIHISNN9ORBMLHZKUEVC9UKQWOXAIA9XLJGESLUKJEKMXCTSLUAERLM9Z"
    "YPOGWULWQKOQOEJNAYGIXCTLRDJIBUOTJNUZQCPQVGASKKYSBMHMTUCXSEUHF9BSDWNPOZTDYV"
    "NNGWIBJUDZQSCMXPGKHNOGAEXVWINWNUHHFVQNXYUR9EQIRLWEJGNZAWOZHJUTOZRPVEGJSQXK"
    "IXMJFDCXRIBBVTYECW9FTPBGTDLPYC9GPMNIWOJBKQTKULMMXCQYBDQKKYYHLRWMRRKADULVHL"
    "ZMWPHLSHX9UWLDYWNHZZEOPYLDJHGTFCTZZKFNSCTTRSLLUVEWHCEUTYOMFPKLAYESZB9WUZYY"
    "LCLAUBBTXLGPOSQE99DLHEWNZLBYVNKBNVLZUXFYODYYQYBIIUSWBNKZDQCCWECDKUKQWQQLIE"
    "NDDNMPZQSKYMXUKPXZMUTVDAZXCGADITLCFXD9ZHOCDCQREXZYADLIICFTIDDAECSEUUNRWXSK"
    "ETYPPXZKIDGWLXD9LRKJIDLFHPIWNTGCX9CLVLHSIPBZJBBFLOVSNVOHUAXQWCWFJPVB9AQEES"
    "XAKZVILAVQFIQQXSWTZGMACSJLXXZ9ZIYQMRGFPFTIEOKTDOOEZFOLLSEBU9VBVNDKH9EZYHXJ"
    "UCT9KQZYMNUCFYOQIAOBZJ9WPULBBMEIZFRDXWOCFEZDCHMMOOMIOCAAPXSBVZKNAQ9PQHSULW"
    "OBNMDKENFZJZBFV9XBRDXFORFWDMXE9CZSHTYIYNRH9ZFORIDH9HXAPHLVCMRACACNNHKFKWKL"
    "EBVVWTDBKWBWLNHKSYWRVWADEVTCLELQOXWHSDEUZKHYSXUYKGRMBEIZOEJZEDGFLOBGUPKPWY"
    "CS9IZQAJABEGPIXWXWUSESOIXTHECBJQMZPTNGWUEGZDDCGONTZUERVSIJIOLQDKKMXPNEGIYE"
    "9CSTU9OPJRFEKLHAANY9XPMSZWWLXNVDKCMGLQYKGAUDJLCVFQBWZTFUBDBCJYIISYLMDHOSFV"
    "RGJALDHYPITFLSMQYXHXWXEPX9ATBUIYRZSBNCUPOQAOYDIDWVUDPFLMLRWNVSHG9YMUSWVZAU"
    "FWDMQQDSAUWCZAXFIJRTOM99IVI9LNWUSFQFRNMENE9JNSGG9ZLEZKZQPCURGBHMRCASDUFBZY"
    "LKHWNOSUBDNDVGNZQLP9RYQKYKDQUCPOIACSLAVFLWXKQPRAPHCQHTRKTFQZODQXIKLDROA9AV"
    "VSMYTXZDFBJRT9TBYHVABUKKRXQPJOBXIG9RCRYWBJLHANXHYOMRNJRFC9PBKDJEPDDGJNETCH"
    "ZNTCVDOSXUBTAKFXQQOEYNNAUNAWODERT9VMHLUTAEESTGQHOQGDSFDWLNYHWWOSXSNMPNCZGP"
    "WYZMUYGFZLSFCJILU9BRAKPLCUKL9S9KG99FQYIROWIDMZBTKLWNHTRKZPBCKBZORNUWAHLYPU"
    "UWMYHAKRNUNUFGCECOXAXSPGATZJLOXMTTXSCWEVPTUFRVKVIGOAPXJPFUFUEJGZEAAIQOQAGF"
    "LG9YMUZAYQOKOZKK9NLRZWUNAIACOCBYYWJVTEGUJKKUBZDZAHFGNKUCHHWLIHMHRRMHBSBJNZ"
    "ZCVSOPJLPPHTPGXXOVQOGQIUBPEKIEAYWZHKNQXZDMBEBFRYNMFGAOFTFBQQARMRLSROUGDKMA"
    "LWQSDZGLVMOZPGNHEV9FMVEDJRZD9OOXMUQRWSNJB",
    "CPKQROLQEATHPLWMRDPIIJHWBDQDUUTFVWQSSDABSEWLIYAWSOUWTMO9ARNLISROQHW9FEMH9Y"
    "E9ZQCOXVNZSHTLHURLZMTVDXJDOKBQBGXOOGSQMKMAZWHBMSZSZOJVQUXRFNXGGOAUIN9YMEDA"
    "QQNDXLDRYYUEJZHVRP9FYPZQWDEDGWK9KYKKBFSRDROLFZBBWUB9DBIRIOSGHMYPEJQYNDOXVF"
    "OQZYTWCFTDMBN9VN9G9TATYOJXQHOVVONMLIYWGADINDDZMPA9J9FNPMIBLFNVEBWGHOPIBUOZ"
    "EEFGHVYKDAVAT9MHACOZKUFTGGRWUKJZDKCWZOLQ9BANRAQAQWRLHDYELUAVJOFZHFCZSVPOEE"
    "HGUITMRZLSBZCCZ9XDMER9TPXJHBZWYO9L9MQFLHAGJ9SRUUANZOQZMRKWAXRNVHNWAFEETVVP"
    "KQOSTSBDKLDSQFWNKLBJECFIRYFV9YVJYKDDVXJRZAZPWSTPBL9RA9CGESRTUYDOHTAEBCOHSJ"
    "PQRDAIARHEBFWZNU9FUGPWIGCGSUUUHQSLAAUHZUQFHQYTRNZDYSBXMAC9JQEAKSHRFCCGVMUL"
    "WUOPHQSQOKZADUWBGYVPWBE9JNTYRMBPMWNFTTBADDBFTUKTAAMEGKDWXHNBHKDUURGTIQNBVX"
    "YHQLLAP9QLXUAHHLOXBKKFCZVRFQSVM9OOEXMFSMBHQISRBGEIXVXJCOUSCUPICRXYBMQBPJSC"
    "LIWHDX9JQAHDDBZBAPQVDEWKARBDYKKRRZXPGFYJSUDZYERWDUVJFSPOJKJY9QJEFAZWRBODXT"
    "9HYJKLDQFPFPXGZLLNFURQXRKKAVGWUDUFEJOBWZUNPNXNBEWCUFDVNTPHIOOHW9HUYDKYRQKV"
    "SKD9YGZLDBQT9ULRQMRQZXXSIHGODYXWGIUMEOIBYUCKKLKAPFUHXRTJBQJAPWTQRYURQICZCQ"
    "FLKMM9W9YXGHTDWTAQADGGPGDSJILXQAOXOFWCWOGKOCD99WHUGXAMFBNBFUVOATVPXMN9DISL"
    "ONXRRTB9SQGBIUURWIXLAYHBI9ORVLNVATGAYTFOLNLYGENLAJETTPWVJWNDERAAFXHWUMUDZJ"
    "N9FPYZMHUVOPWOO99AE9UNIBZICBOYOTTMOZWWJPRBWAE9HTBPAE9HDVGKPXITTJYDRDXVBZKF"
    "E9EKFGOWNY9WHMDTSYIETYNZUUBTEGAGDLVSUJWNBKHOFOGQMVABVZJXWZ9SQLBRFMUMBWBRVU"
    "PVFJRCZUVQLNEUYMHBZODLDTOVGXCAZCIBHHHYX9MTX9CNTGGXGCYWWMHACHHIFEG9JRCSTGNF"
    "TBBZBFXTOF9B9LENFPFBEZQZZILGKQYTONOBXONDBIM9DZEYTVVHZFOSNKVMQILXQRGUEITRCN"
    "F9QJXVMSR9LUJFENFQPGKKAIVZDFXBGAZUCGCOXLUOHAAGFJKSGWROPYUYRSNAJRIDYWRWTVCL"
    "VMRBIFUZCQCKEEPKZPXDYHSGNHEUCNAPTMQPYNQTLW9MPOEQEZYOUZOLPFDRHULRJBNGWIIQRU"
    "DSQYF9AJEQZJOKBPLQWKN9DNXYL9IDOHXWFEQXLRQDIJUSMMUSOQLEXSPUMWLBUYMZXUZYCKLT"
    "9SEVXPSZWERLUPQWGDEGXUAPJNLMKUAAWZZKOHUNQMIHESGI9CMPXSQIDJBIQNCDSUQLUYFKYC"
    "WWUHEFDXEVIERJMVBTQRIGKU9JDOQNW9RUCHHQVUU9LSUEGFXMUIEMG9UFUQJIUT9IUNLHQKUT"
    "BBEDQWELSBUMNDUSVBRJZK9LENSAPM9FRHRIJNNIKYIVYQJUSLKHGJQKOKIIOWAUOZRGZTQXTV"
    "LTTGCY9MAKCLZGFGLVBSCB99EJ99PRGJRDSDO9AT9NNM9DNPNQXBVVOTTREJKEOYZKSDDEHZEJ"
    "FLKTKLKDRXAKWQFDM9OBISSPCMU9JYHWWEFXICKGHPSMBFLOCKUKIRWHSBAJXVUPLCMEZCHXQ9"
    "DPHJN99WJOOOMRWM9CFMXKVAXR9RFPANVWGRAUKZRZPGEEXYMCJFWTUHFTUPEIDFHXLAFG9NMN"
    "CWQDPIVXOXB9WHGEVOTYXANITICWGTFTEWLVXFSIQQUSQEKUJTUOLBIWOIFNQOIHUVTCEAUVMM"
    "YVYTLBFXSBIISGCAWSQJPEUZBMTEBXMUBVNCEFFMB"};

static void generate_signature(const unsigned char *seed_bytes,
                               uint32_t address_idx, uint8_t security,
                               char *signature)
{
    SIGNING_CTX ctx;
    unsigned char signature_fragment[SIGNATURE_FRAGMENT_SIZE * NUM_HASH_BYTES];

    signing_initialize(&ctx, 0, seed_bytes, address_idx, security,
                       NORMALIZED_HASH);

    for (int i = 0; i < NUM_SIGNATURE_FRAGMENTS(security); i++) {

        const int fragment_index =
            signing_next_fragment(&ctx, signature_fragment);
        assert_int_equal(fragment_index, i);

        bytes_to_chars(signature_fragment,
                       signature + i * SIGNATURE_FRAGMENT_SIZE * 81,
                       SIGNATURE_FRAGMENT_SIZE * NUM_HASH_BYTES);
    }
}

static void assert_signature_equals(uint8_t security, const char *signature)
{
    for (unsigned int i = 0; i < security; i++) {
        // create zero terminated fragment
        const size_t fragment_length = strlen(FRAGMENTS[i]);
        char fragment[fragment_length + 1];
        memcpy(fragment, signature + i * 27 * NUM_HASH_TRYTES, fragment_length);
        fragment[fragment_length] = '\0';

        assert_string_equal(fragment, FRAGMENTS[i]);
    }
}

static void test_signature_level_one(void **state)
{
    UNUSED(state);

    const uint8_t security = 1;

    unsigned char seed_bytes[NUM_HASH_BYTES];
    chars_to_bytes(PETER_SEED, seed_bytes, NUM_HASH_TRYTES);

    char signature[MAX_SIGNATURE_LENGTH + 1] = {0};
    generate_signature(seed_bytes, IDX, security, signature);

    assert_signature_equals(security, signature);
}

static void test_signature_level_two(void **state)
{
    UNUSED(state);

    const uint8_t security = 2;

    unsigned char seed_bytes[NUM_HASH_BYTES];
    chars_to_bytes(PETER_SEED, seed_bytes, NUM_HASH_TRYTES);

    char signature[MAX_SIGNATURE_LENGTH + 1] = {0};
    generate_signature(seed_bytes, IDX, security, signature);

    assert_signature_equals(security, signature);
}

static void test_signature_level_three(void **state)
{
    UNUSED(state);

    const uint8_t security = 3;

    unsigned char seed_bytes[NUM_HASH_BYTES];
    chars_to_bytes(PETER_SEED, seed_bytes, NUM_HASH_TRYTES);

    char signature[MAX_SIGNATURE_LENGTH + 1] = {0};
    generate_signature(seed_bytes, IDX, security, signature);

    assert_signature_equals(security, signature);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_signature_level_one),
        cmocka_unit_test(test_signature_level_two),
        cmocka_unit_test(test_signature_level_three)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

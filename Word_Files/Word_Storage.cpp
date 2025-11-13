#include "Word_Files/Word_Storage.h"
#include "Qt_includes.h"

WordStorage &WordStorage::instance()
{
    static WordStorage s;
    return s;
}

bool WordStorage::load(const QString &path)
{
    m_path = path.isEmpty() ? QString("words.json") : path;
    m_words.clear();

    // Ensure initial in-code words are present first.
    insertInitialWords();

    QFile f(m_path);
    if (!f.exists()) {
        // No file yet: persist the initial in-code words to disk.
        QDir().mkpath(QFileInfo(m_path).absolutePath());
        save();
        return true;
    }

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    f.close();

    if (!doc.isArray()) return false;

    // Load file entries but avoid duplicating words already provided by
    // insertInitialWords() (case-insensitive match by word text).
    QJsonArray arr = doc.array();
    for (const auto &v : arr) {
        if (!v.isObject()) continue;
        WordEntry entry = WordEntry::fromJson(v.toObject());

        bool exists = false;
        for (const auto &w : m_words) {
            if (w.word.compare(entry.word, Qt::CaseInsensitive) == 0) {
                exists = true;
                break;
            }
        }
        if (!exists) m_words.append(entry);
    }
    return true;
}

bool WordStorage::save(const QString &path)
{
    QString p = path.isEmpty() ? m_path : path;
    if (p.isEmpty()) return false;

    QJsonArray arr;
    for (const auto &w : m_words) arr.append(w.toJson());

    QJsonDocument doc(arr);
    QFile f(p);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    f.write(doc.toJson(QJsonDocument::Indented));
    f.close();

    return true;
}

void WordStorage::addWord(const WordEntry &entry)
{
    for (const auto &w : m_words) {
        if (w.word.compare(entry.word, Qt::CaseInsensitive) == 0) return;
    }
    m_words.append(entry);
}

QVector<WordEntry> WordStorage::allWords() const { return m_words; }

QVector<WordEntry> WordStorage::wordsForLetter(QChar letter) const
{
    QVector<WordEntry> out;
    if (letter.isNull()) return out;

    QChar L = letter.toLower();
    for (const auto &w : m_words) {
        if (w.word.isEmpty()) continue;
        if (w.word.at(0).toLower() == L) out.append(w);
    }
    return out;
}

void WordStorage::insertInitialWords()
{
    m_words.clear();
    WordEntry e;

    // Letter A Words
    e.word = "Abandon";
    e.definition = "He had to abandon his plans when the storm hit.";
    e.translation = "Iwan\nKasingkahulugan: talikuran, pabayaan, isuko\nKasalungat: panatilihin, ingatan, yakapin\nHalimbawa: Kinailangan niyang iwan ang kanyang mga plano nang dumating ang bagyo.";
    e.synonyms = {"desert", "forsake", "relinquish"};
    e.antonyms = {"retain", "keep", "embrace"};
    e.background = "Origin: Old French 'abandoner', to surrender";
    e.usage = "He had to abandon his plans when the storm hit.";
    m_words.append(e);

    e.word = "Abolish";
    e.definition = "The law was abolished after it was deemed unjust.";
    e.translation = "Alisin (Pawiin)\nKasingkahulugan: burahin, alisin, kanselahin\nKasalungat: itatag, likhain, panatilihin\nHalimbawa: Inalis ang batas matapos itong ideklarang hindi makatarungan.";
    e.synonyms = {"eradicate", "eliminate", "cancel"};
    e.antonyms = {"establish", "create", "uphold"};
    e.background = "Origin: Latin 'abolere', to destroy";
    e.usage = "The law was abolished after it was deemed unjust.";
    m_words.append(e);

    e.word = "Absorb";
    e.definition = "The sponge will absorb all the water from the spill.";
    e.translation = "Sumipsip\nKasingkahulugan: sumipsip, tanggapin, lamunin\nKasalungat: ilabas, pakawalan, palabasin\nHalimbawa: Sisisipsipin ng espongha ang lahat ng tubig sa natapon.";
    e.synonyms = {"soak up", "take in", "assimilate"};
    e.antonyms = {"expel", "release", "discharge"};
    e.background = "Origin: Latin 'absorbere', to swallow up";
    e.usage = "The sponge will absorb all the water from the spill.";
    m_words.append(e);

    e.word = "Abundant";
    e.definition = "The region is abundant in natural resources.";
    e.translation = "Masagana\nKasingkahulugan: marami, sagana, labis\nKasalungat: kulang, limitado, kapos\nHalimbawa: Sagana ang rehiyon sa likas na yaman.";
    e.synonyms = {"plentiful", "ample", "copious"};
    e.antonyms = {"scarce", "limited", "inadequate"};
    e.background = "Origin: Latin 'abundare', to overflow";
    e.usage = "The region is abundant in natural resources.";
    m_words.append(e);

    e.word = "Accelerate";
    e.definition = "The car started to accelerate as we drove downhill.";
    e.translation = "Pabilisin\nKasingkahulugan: pabilisin, paharapin, magmadali\nKasalungat: pabagalin, huminto, ipagpaliban\nHalimbawa: Nagsimulang pabilisin ng kotse ang takbo pababa ng burol.";
    e.synonyms = {"speed up", "hasten", "quicken"};
    e.antonyms = {"slow down", "decelerate", "delay"};
    e.background = "Origin: Latin 'accelerare', to hasten";
    e.usage = "The car started to accelerate as we drove downhill.";
    m_words.append(e);

    e.word = "Accessible";
    e.definition = "The library is accessible to everyone, including people with disabilities.";
    e.translation = "Madaling Mapuntahan\nKasingkahulugan: maabot, makuha, magamit\nKasalungat: hindi maabot, sarado, harang\nHalimbawa: Madaling mapuntahan ng lahat ang aklatan, kabilang ang may kapansanan.";
    e.synonyms = {"reachable", "attainable", "available"};
    e.antonyms = {"inaccessible", "unreachable", "blocked"};
    e.background = "Origin: Latin 'accessibilis', easy to reach";
    e.usage = "The library is accessible to everyone, including people with disabilities.";
    m_words.append(e);

    e.word = "Accomplish";
    e.definition = "She managed to accomplish all her goals for the year.";
    e.translation = "Matupad\nKasingkahulugan: makamit, matapos, maganap\nKasalungat: mabigo, mapalampas, pabayaan\nHalimbawa: Nagawa niyang matupad ang lahat ng kanyang mga layunin sa taon.";
    e.synonyms = {"achieve", "complete", "fulfill"};
    e.antonyms = {"fail", "miss", "neglect"};
    e.background = "Origin: Middle English 'accomplisshen', to achieve";
    e.usage = "She managed to accomplish all her goals for the year.";
    m_words.append(e);

    e.word = "Accurate";
    e.definition = "The test results were accurate and showed no errors.";
    e.translation = "Tumpak\nKasingkahulugan: tama, eksakto, wasto\nKasalungat: mali, hindi tama, malabo\nHalimbawa: Tumpak ang mga resulta ng pagsusuri at walang mali.";
    e.synonyms = {"correct", "precise", "exact"};
    e.antonyms = {"inaccurate", "incorrect", "imprecise"};
    e.background = "Origin: Latin 'accuratus', well done";
    e.usage = "The test results were accurate and showed no errors.";
    m_words.append(e);

    // Letter B Words
    e.word = "Balance";
    e.definition = "She struggled to find a balance between work and personal life.";
    e.translation = "Balanse\nKasingkahulugan: pagkakapantay, katatagan, simetriya\nKasalungat: kawalan ng balanse, kawalang-tatag, hindi pagkakatugma\nHalimbawa: Nahirapan siyang hanapin ang balanse sa pagitan ng trabaho at personal na buhay.";
    e.synonyms = {"equilibrium", "stability", "symmetry"};
    e.antonyms = {"imbalance", "instability", "disproportion"};
    e.background = "Origin: Latin 'bilanx', having two pans";
    e.usage = "She struggled to find a balance between work and personal life.";
    m_words.append(e);

    e.word = "Banish";
    e.definition = "The king decided to banish the traitor from the kingdom.";
    e.translation = "Itaboy\nKasingkahulugan: ipatapon, paalisin, palayasin\nKasalungat: tanggapin, anyayahan, papasukin\nHalimbawa: Nagpasya ang hari na itaboy ang taksil mula sa kaharian.";
    e.synonyms = {"exile", "expel", "eject"};
    e.antonyms = {"welcome", "invite", "admit"};
    e.background = "Origin: Old French 'banir', to proclaim";
    e.usage = "The king decided to banish the traitor from the kingdom.";
    m_words.append(e);

    e.word = "Bare";
    e.definition = "The bare walls of the room made it feel cold and empty.";
    e.translation = "Hubad\nKasingkahulugan: walang takip, lantad, hubo\nKasalungat: may damit, natatakpan, protektado\nHalimbawa: Ang hubad na mga pader ng silid ay nagbigay dito ng malamig na pakiramdam.";
    e.synonyms = {"exposed", "uncovered", "nude"};
    e.antonyms = {"clothed", "covered", "protected"};
    e.background = "Origin: Old English 'baer', naked";
    e.usage = "The bare walls of the room made it feel cold and empty.";
    m_words.append(e);

    e.word = "Benevolent";
    e.definition = "The benevolent king always helped the poor.";
    e.translation = "Mapagkawanggawa\nKasingkahulugan: mabait, maawain, matulungin\nKasalungat: malupit, masama, walang puso\nHalimbawa: Ang mapagkawanggawang hari ay laging tumutulong sa mahihirap.";
    e.synonyms = {"kind", "charitable", "compassionate"};
    e.antonyms = {"malevolent", "unkind", "cruel"};
    e.background = "Origin: Latin 'benevolus', wishing well";
    e.usage = "The benevolent king always helped the poor.";
    m_words.append(e);

    e.word = "Brave";
    e.definition = "The brave firefighter ran into the burning building to rescue the family.";
    e.translation = "Matapang\nKasingkahulugan: walang takot, magiting, matibay ang loob\nKasalungat: duwag, mahina ang loob, natatakot\nHalimbawa: Ang matapang na bumbero ay pumasok sa nasusunog na gusali upang iligtas ang pamilya.";
    e.synonyms = {"courageous", "fearless", "valiant"};
    e.antonyms = {"cowardly", "timid", "fearful"};
    e.background = "Origin: Italian 'bravo', wild or fierce";
    e.usage = "The brave firefighter ran into the burning building to rescue the family.";
    m_words.append(e);

    e.word = "Bland";
    e.definition = "The soup was too bland for my taste; it needed more seasoning.";
    e.translation = "Maputla\nKasingkahulugan: matabang, walang lasa, walang sigla\nKasalungat: malasa, maanghang, masigla\nHalimbawa: Masyadong maputla ang sabaw para sa panlasa ko; kailangan pa ng pampalasa.";
    e.synonyms = {"dull", "tasteless", "insipid"};
    e.antonyms = {"flavorful", "spicy", "exciting"};
    e.background = "Origin: Latin 'blandus', smooth or flattering";
    e.usage = "The soup was too bland for my taste; it needed more seasoning.";
    m_words.append(e);

    e.word = "Blissful";
    e.definition = "They spent a blissful weekend in the mountains.";
    e.translation = "Masaya\nKasingkahulugan: maligaya, kuntento, masigla\nKasalungat: malungkot, balisa, nagdadalamhati\nHalimbawa: Nagtamasa sila ng masayang weekend sa kabundukan.";
    e.synonyms = {"happy", "content", "joyful"};
    e.antonyms = {"miserable", "unhappy", "sorrowful"};
    e.background = "Origin: Old English 'blisse', joy";
    e.usage = "They spent a blissful weekend in the mountains.";
    m_words.append(e);

    e.word = "Blunt";
    e.definition = "Her blunt response to the criticism surprised everyone in the room.";
    e.translation = "Diretso\nKasingkahulugan: prangka, tapat, walang paligoy\nKasalungat: maingat, diplomatikong, maselan\nHalimbawa: Ang prangka niyang sagot sa puna ay ikinagulat ng lahat sa silid.";
    e.synonyms = {"direct", "straightforward", "frank"};
    e.antonyms = {"tactful", "diplomatic", "subtle"};
    e.background = "Origin: Scandinavian 'blunt', dull or blunt edge";
    e.usage = "Her blunt response to the criticism surprised everyone in the room.";
    m_words.append(e);

    e.word = "Bore";
    e.definition = "The meeting lasted for hours and really started to bore me.";
    e.translation = "Nakakainip\nKasingkahulugan: nakakapagod, nakakaantok, walang sigla\nKasalungat: nakakaaliw, nakakatawa, interesante\nHalimbawa: Napakatagal ng pulong at talagang nakakainip.";
    e.synonyms = {"tire", "weary", "dull"};
    e.antonyms = {"entertain", "amuse", "interest"};
    e.background = "Origin: Old French 'borer', to drill";
    e.usage = "The meeting lasted for hours and really started to bore me.";
    m_words.append(e);

    e.word = "Brisk";
    e.definition = "She took a brisk walk in the park to get some fresh air.";
    e.translation = "Masigla\nKasingkahulugan: maliksi, mabilis, buhay na buhay\nKasalungat: mabagal, matamlay, tamad\nHalimbawa: Naglakad siya nang masigla sa parke para makalanghap ng sariwang hangin.";
    e.synonyms = {"energetic", "quick", "lively"};
    e.antonyms = {"sluggish", "slow", "lethargic"};
    e.background = "Origin: Scandinavian, meaning sharp or biting";
    e.usage = "She took a brisk walk in the park to get some fresh air.";
    m_words.append(e);

    e.word = "Bitter";
    e.definition = "The bitter argument left both of them upset and frustrated.";
    e.translation = "Mapait\nKasingkahulugan: marahas, masakit, may hinanakit\nKasalungat: matamis, magaan, maganda\nHalimbawa: Ang mapait na pagtatalo ay nagdulot ng sama ng loob sa magkabilang panig.";
    e.synonyms = {"harsh", "sour", "resentful"};
    e.antonyms = {"sweet", "pleasant", "mild"};
    e.background = "Origin: Old English 'biter', to bite";
    e.usage = "The bitter argument left both of them upset and frustrated.";
    m_words.append(e);

    e.word = "Bigotry";
    e.definition = "Bigotry has no place in a society that values equality.";
    e.translation = "Pagkapanatiko\nKasingkahulugan: pagkiling, pagtatangi, diskriminasyon\nKasalungat: pagiging bukas, pagtanggap, katarungan\nHalimbawa: Walang lugar ang pagkapanatiko sa lipunang nagmamahal sa pagkakapantay-pantay.";
    e.synonyms = {"intolerance", "prejudice", "discrimination"};
    e.antonyms = {"open-mindedness", "acceptance", "fairness"};
    e.background = "Origin: French 'bigoterie', derived from Bigos";
    e.usage = "Bigotry has no place in a society that values equality.";
    m_words.append(e);

    e.word = "Baffled";
    e.definition = "She was baffled by the strange behavior of her friend.";
    e.translation = "Nalito\nKasingkahulugan: naguluhan, nagtataka, nagtaka\nKasalungat: tiyak, malinaw, sigurado\nHalimbawa: Nalito siya sa kakaibang asal ng kanyang kaibigan.";
    e.synonyms = {"confused", "puzzled", "bewildered"};
    e.antonyms = {"certain", "clear", "sure"};
    e.background = "Origin: Scottish, meaning to check or repel";
    e.usage = "She was baffled by the strange behavior of her friend.";
    m_words.append(e);

    e.word = "Benevolence";
    e.definition = "His acts of benevolence made him beloved by all.";
    e.translation = "Kabaitan\nKasingkahulugan: kagandahang-loob, kabutihan, pagkamaawain\nKasalungat: kalupitan, kasamaan, pagiging makasarili\nHalimbawa: Ang kanyang kabaitan ay minahal ng lahat.";
    e.synonyms = {"kindness", "goodwill", "generosity"};
    e.antonyms = {"selfishness", "cruelty", "malevolence"};
    e.background = "Origin: Latin 'benevolentia', desire to do good";
    e.usage = "His acts of benevolence made him beloved by all.";
    m_words.append(e);

    e.word = "Brittle";
    e.definition = "The glass vase was brittle and broke into pieces with the slightest touch.";
    e.translation = "Marupok\nKasingkahulugan: madaling mabasag, mahina, maramdamin\nKasalungat: matibay, matatag, matigas\nHalimbawa: Ang basong iyon ay marupok at agad nabasag nang mahulog.";
    e.synonyms = {"fragile", "delicate", "weak"};
    e.antonyms = {"durable", "strong", "resilient"};
    e.background = "Origin: Old English 'breotan', to break";
    e.usage = "The glass vase was brittle and broke into pieces with the slightest touch.";
    m_words.append(e);

    e.word = "Brilliant";
    e.definition = "The scientist's brilliant discovery changed the field forever.";
    e.translation = "Matalino\nKasingkahulugan: magaling, mahusay, henyo\nKasalungat: bobo, mahina, walang alam\nHalimbawa: Ang matalinong tuklas ng siyentipiko ay nagbago ng larangan magpakailanman.";
    e.synonyms = {"outstanding", "exceptional", "talented"};
    e.antonyms = {"dull", "mediocre", "uninspired"};
    e.background = "Origin: Italian 'brillare', to shine";
    e.usage = "The scientist's brilliant discovery changed the field forever.";
    m_words.append(e);

    e.word = "Bounty";
    e.definition = "The harvest provided a bounty of fruits and vegetables.";
    e.translation = "Kasaganaan\nKasingkahulugan: kayamanan, kasapatan, kasiyahan\nKasalungat: kakulangan, kakapusan, paghihirap\nHalimbawa: Ang aning ito ay nagdala ng kasaganaan ng prutas at gulay.";
    e.synonyms = {"abundance", "plenty", "wealth"};
    e.antonyms = {"scarcity", "shortage", "lack"};
    e.background = "Origin: Old French 'bonte', goodness";
    e.usage = "The harvest provided a bounty of fruits and vegetables.";
    m_words.append(e);

    e.word = "Blaze";
    e.definition = "The blaze of the campfire kept us warm on the cold night.";
    e.translation = "Apoy\nKasingkahulugan: siga, liyab, alab\nKasalungat: puksain, patayin, apulahin\nHalimbawa: Ang apoy ng bonfire ang nagbigay-init sa malamig na gabi.";
    e.synonyms = {"fire", "flame", "inferno"};
    e.antonyms = {"extinguish", "douse", "put out"};
    e.background = "Origin: Old English 'blæse', white mark";
    e.usage = "The blaze of the campfire kept us warm on the cold night.";
    m_words.append(e);

    e.word = "Baffle";
    e.definition = "The magician's trick completely baffled the audience.";
    e.translation = "Malito\nKasingkahulugan: maguluhan, malabuan, mabigla\nKasalungat: malinawan, maunawaan, masimplihan\nHalimbawa: Lubos na nalito ang mga tao sa mahikang ipinakita ng salamangkero.";
    e.synonyms = {"confuse", "perplex", "bewilder"};
    e.antonyms = {"clarify", "explain", "simplify"};
    e.background = "Origin: Scottish, to check or repel";
    e.usage = "The magician's trick completely baffled the audience.";
    m_words.append(e);

    e.word = "Brawl";
    e.definition = "The two men got into a brawl outside the bar.";
    e.translation = "Gulo\nKasingkahulugan: suntukan, sagupaan, alitan\nKasalungat: kapayapaan, pagkakaisa, katahimikan\nHalimbawa: Nagkaroon ng gulo sa labas ng bar dahil sa dalawang lalaki.";
    e.synonyms = {"fight", "clash", "scuffle"};
    e.antonyms = {"peace", "harmony", "calm"};
    e.background = "Origin: Middle Dutch 'bralle', to brawl";
    e.usage = "The two men got into a brawl outside the bar.";
    m_words.append(e);

    e.word = "Bright";
    e.definition = "The future looks bright for young professionals in this field.";
    e.translation = "Maliwanag\nKasingkahulugan: maningning, makintab, kumikislap\nKasalungat: madilim, mapusyaw, walang ilaw\nHalimbawa: Maliwanag ang kinabukasan ng mga kabataang propesyonal sa larangang ito.";
    e.synonyms = {"radiant", "brilliant", "shining"};
    e.antonyms = {"dull", "dim", "dark"};
    e.background = "Origin: Old English 'beorht', shining";
    e.usage = "The future looks bright for young professionals in this field.";
    m_words.append(e);

    e.word = "Blunder";
    e.definition = "He made a huge blunder during the presentation by forgetting his key points.";
    e.translation = "Pagkakamali\nKasingkahulugan: error, pagkukulang, pagkadulas\nKasalungat: tagumpay, tama, katuparan\nHalimbawa: Nagkaroon siya ng malaking pagkakamali sa presentasyon nang makalimutan ang mahahalagang punto.";
    e.synonyms = {"mistake", "error", "misstep"};
    e.antonyms = {"success", "achievement", "triumph"};
    e.background = "Origin: Scandinavian 'blunda', to doze";
    e.usage = "He made a huge blunder during the presentation by forgetting his key points.";
    m_words.append(e);

    e.word = "Bizarre";
    e.definition = "His bizarre behavior left everyone in the office confused.";
    e.translation = "Kakaiba\nKasingkahulugan: pambihira, hindi karaniwan, nakapagtataka\nKasalungat: normal, karaniwan, pangkaraniwan\nHalimbawa: Ang kakaibang kilos niya ay nakagulat sa lahat sa opisina.";
    e.synonyms = {"strange", "odd", "peculiar"};
    e.antonyms = {"normal", "conventional", "typical"};
    e.background = "Origin: Spanish 'bizarro', brave or fierce";
    e.usage = "His bizarre behavior left everyone in the office confused.";
    m_words.append(e);

    e.word = "Breezy";
    e.definition = "The breezy afternoon made the beach a perfect spot to relax.";
    e.translation = "Mahangin\nKasingkahulugan: presko, maaliwalas, may simoy\nKasalungat: kalmado, walang hangin, tahimik\nHalimbawa: Ang mahangin na hapon ay perpekto para magpahinga sa tabing-dagat.";
    e.synonyms = {"windy", "fresh", "airy"};
    e.antonyms = {"calm", "still", "quiet"};
    e.background = "Origin: English 'breeze', light wind";
    e.usage = "The breezy afternoon made the beach a perfect spot to relax.";
    m_words.append(e);

    e.word = "Bumpy";
    e.definition = "The road was bumpy, making the ride uncomfortable.";
    e.translation = "Lubak-lubak\nKasingkahulugan: hindi pantay, magaspang, mabundok\nKasalungat: makinis, pantay, maayos\nHalimbawa: Lubak-lubak ang kalsada kaya hindi komportable ang biyahe.";
    e.synonyms = {"uneven", "rough", "rugged"};
    e.antonyms = {"smooth", "level", "even"};
    e.background = "Origin: English 'bump', a raised mass";
    e.usage = "The road was bumpy, making the ride uncomfortable.";
    m_words.append(e);

    e.word = "Boost";
    e.definition = "The new advertising campaign helped boost sales.";
    e.translation = "Palakasin\nKasingkahulugan: dagdagan, paunlarin, pataasin\nKasalungat: bawasan, pahinain, pababain\nHalimbawa: Ang bagong kampanya sa marketing ay nakatulong para palakasin ang benta.";
    e.synonyms = {"increase", "raise", "enhance"};
    e.antonyms = {"decrease", "diminish", "reduce"};
    e.background = "Origin: English 'boost', to push up";
    e.usage = "The new advertising campaign helped boost sales.";
    m_words.append(e);

    e.word = "Bold";
    e.definition = "His bold decision to start a new business paid off in the end.";
    e.translation = "Matapang\nKasingkahulugan: walang takot, palaban, mapangahas\nKasalungat: mahina, duwag, matatakutin\nHalimbawa: Ang matapang niyang desisyon na magsimula ng negosyo ay nagbunga ng tagumpay.";
    e.synonyms = {"daring", "courageous", "fearless"};
    e.antonyms = {"timid", "cautious", "afraid"};
    e.background = "Origin: Old English 'bald', confident";
    e.usage = "His bold decision to start a new business paid off in the end.";
    m_words.append(e);

    e.word = "Bashful";
    e.definition = "The bashful child hid behind his mother when meeting strangers.";
    e.translation = "Mahiyain\nKasingkahulugan: mailap, tahimik, mahina ang loob\nKasalungat: palakaibigan, kumpiyansa, matapang\nHalimbawa: Ang mahiyain na bata ay nagtago sa likod ng kanyang ina nang makilala ang bagong tao.";
    e.synonyms = {"shy", "timid", "self-conscious"};
    e.antonyms = {"outgoing", "confident", "bold"};
    e.background = "Origin: English 'bash', to strike";
    e.usage = "The bashful child hid behind his mother when meeting strangers.";
    m_words.append(e);

    e.word = "Beaming";
    e.definition = "She walked into the room with a beaming smile on her face.";
    e.translation = "Nakangiti\nKasingkahulugan: masigla, maliwanag, masaya\nKasalungat: malungkot, matamlay, seryoso\nHalimbawa: Pumasok siya sa silid na may nakangiting mukha.";
    e.synonyms = {"radiant", "glowing", "cheerful"};
    e.antonyms = {"gloomy", "sad", "downcast"};
    e.background = "Origin: English 'beam', a ray of light";
    e.usage = "She walked into the room with a beaming smile on her face.";
    m_words.append(e);

    e.word = "Bountiful";
    e.definition = "The garden produced a bountiful harvest this year.";
    e.translation = "Masagana\nKasingkahulugan: marami, sagana, mapagbigay\nKasalungat: kulang, kakaunti, limitado\nHalimbawa: Ang hardin ay nagbunga ng masaganang ani ngayong taon.";
    e.synonyms = {"plentiful", "abundant", "generous"};
    e.antonyms = {"scarce", "insufficient", "limited"};
    e.background = "Origin: Old French 'bonte', goodness";
    e.usage = "The garden produced a bountiful harvest this year.";
    m_words.append(e);

    e.word = "Brutal";
    e.definition = "The brutal truth was hard to hear but necessary.";
    e.translation = "Malupit\nKasingkahulugan: marahas, walang awa, brutal\nKasalungat: mabait, maawain, banayad\nHalimbawa: Mahirap tanggapin ang malupit na katotohanan, ngunit ito ay kailangan.";
    e.synonyms = {"savage", "cruel", "harsh"};
    e.antonyms = {"gentle", "kind", "compassionate"};
    e.background = "Origin: Latin 'brutus', dull or stupid";
    e.usage = "The brutal truth was hard to hear but necessary.";
    m_words.append(e);

    e.word = "Befriend";
    e.definition = "He tried to befriend the new student by offering help with her homework.";
    e.translation = "Makipagkaibigan\nKasingkahulugan: tumulong, sumuporta, makisama\nKasalungat: lumaban, tumanggi, kontrahin\nHalimbawa: Sinubukan niyang makipagkaibigan sa bagong estudyante sa pamamagitan ng pagtulong sa takdang-aralin.";
    e.synonyms = {"ally", "support", "assist"};
    e.antonyms = {"antagonize", "reject", "oppose"};
    e.background = "Origin: English 'friend', a person one knows well";
    e.usage = "He tried to befriend the new student by offering help with her homework.";
    m_words.append(e);

    e.word = "Bliss";
    e.definition = "They lived in bliss for many years after their wedding.";
    e.translation = "Kaligayahan\nKasingkahulugan: saya, tuwa, kasiyahan\nKasalungat: kalungkutan, dalamhati, pighati\nHalimbawa: Nabuhay sila sa kaligayahan matapos ang kanilang kasal.";
    e.synonyms = {"happiness", "joy", "contentment"};
    e.antonyms = {"misery", "sorrow", "sadness"};
    e.background = "Origin: Old English 'blisse', joy";
    e.usage = "They lived in bliss for many years after their wedding.";
    m_words.append(e);

    e.word = "Bash";
    e.definition = "He gave the door a bash with the hammer, trying to fix it.";
    e.translation = "Hampas\nKasingkahulugan: suntok, palo, bugbog\nKasalungat: tapik, tulak, dampi\nHalimbawa: Hinampas niya ang pinto gamit ang martilyo upang ayusin ito.";
    e.synonyms = {"strike", "hit", "slam"};
    e.antonyms = {"tap", "poke", "nudge"};
    e.background = "Origin: Scandinavian 'base', a blow";
    e.usage = "He gave the door a bash with the hammer, trying to fix it.";
    m_words.append(e);

    e.word = "Ban";
    e.definition = "The school decided to ban cell phones during class.";
    e.translation = "Ipinagbawal\nKasingkahulugan: ipagbawal, ipatigil, ipahinto\nKasalungat: payagan, pahintulutan, pahintulot\nHalimbawa: Nagpasya ang paaralan na ipagbawal ang paggamit ng cellphone sa klase.";
    e.synonyms = {"prohibit", "forbid", "outlaw"};
    e.antonyms = {"allow", "permit", "authorize"};
    e.background = "Origin: Old Norse 'banna', to forbid";
    e.usage = "The school decided to ban cell phones during class.";
    m_words.append(e);

    e.word = "Befuddle";
    e.definition = "The complicated instructions befuddled the new employees.";
    e.translation = "Lituhin\nKasingkahulugan: guluhin, lituhin, gulantangin\nKasalungat: ipaliwanag, linawin, payapain\nHalimbawa: Nalito ang mga bagong empleyado sa komplikadong tagubilin.";
    e.synonyms = {"confuse", "perplex", "bewilder"};
    e.antonyms = {"clarify", "explain", "simplify"};
    e.background = "Origin: English 'befuddle', to confuse utterly";
    e.usage = "The complicated instructions befuddled the new employees.";
    m_words.append(e);

    e.word = "Bristle";
    e.definition = "His anger made his hair bristle with frustration.";
    e.translation = "Tindig-Balahibo\nKasingkahulugan: tumayo, manigas, magalit\nKasalungat: mag-relax, lumambot, kukalma\nHalimbawa: Tumindig ang kanyang balahibo sa galit.";
    e.synonyms = {"stiffen", "stand up", "flare"};
    e.antonyms = {"relax", "soften", "calm"};
    e.background = "Origin: Old English 'byrst', to burst";
    e.usage = "His anger made his hair bristle with frustration.";
    m_words.append(e);

    e.word = "Banishment";
    e.definition = "The punishment for breaking the rules was banishment.";
    e.translation = "Pagkatapon\nKasingkahulugan: pagpapatalsik, pagpapaalis, deportasyon\nKasalungat: pagbabalik, pagtanggap, pagbabalik-tanaw\nHalimbawa: Ang parusa sa krimen ay pagkatapon sa komunidad.";
    e.synonyms = {"exile", "expulsion", "deportation"};
    e.antonyms = {"admission", "welcome", "acceptance"};
    e.background = "Origin: Old French 'banir', to proclaim";
    e.usage = "The punishment for breaking the rules was banishment.";
    m_words.append(e);

    e.word = "Bait";
    e.definition = "The fisherman used worms as bait to catch the fish.";
    e.translation = "Pang-akit\nKasingkahulugan: pain, tukso, panghila\nKasalungat: panglayo, pangtaboy, pang-iwas\nHalimbawa: Gumamit ang mangingisda ng bulate bilang pain sa pangingisda.";
    e.synonyms = {"lure", "entice", "attract"};
    e.antonyms = {"discourage", "repel", "deter"};
    e.background = "Origin: Old Norse 'beita', to feed";
    e.usage = "The fisherman used worms as bait to catch the fish.";
    m_words.append(e);

    e.word = "Braggart";
    e.definition = "He's such a braggart that no one likes to talk to him.";
    e.translation = "Mayabang\nKasingkahulugan: palalo, mapagmataas, nagyayabang\nKasalungat: mapagkumbaba, mahinhin, maamo\nHalimbawa: Napakayabang niya kaya walang gustong makipag-usap sa kanya.";
    e.synonyms = {"boaster", "show-off", "egotist"};
    e.antonyms = {"humble", "modest", "reserved"};
    e.background = "Origin: Old French 'braguete', boasting";
    e.usage = "He's such a braggart that no one likes to talk to him.";
    m_words.append(e);

    e.word = "Befit";
    e.definition = "The luxury hotel was a perfect place to befit her status.";
    e.translation = "Bagay\nKasingkahulugan: akma, nararapat, tugma\nKasalungat: hindi tugma, salungat, taliwas\nHalimbawa: Ang marangyang hotel ay bagay sa kanyang katayuan.";
    e.synonyms = {"suit", "be appropriate", "fit"};
    e.antonyms = {"misfit", "clash", "mismatch"};
    e.background = "Origin: Old English 'befittan', to make suitable";
    e.usage = "The luxury hotel was a perfect place to befit her status.";
    m_words.append(e);

    e.word = "Breach";
    e.definition = "The company was sued for a breach of contract.";
    e.translation = "Paglabag\nKasingkahulugan: pagkakasala, pagsuway, paglabag\nKasalungat: pagsunod, pagtalima, paggalang\nHalimbawa: Dinimanda ang kompanya dahil sa paglabag sa kontrata.";
    e.synonyms = {"violation", "infraction", "break"};
    e.antonyms = {"compliance", "observance", "respect"};
    e.background = "Origin: Old French 'breche', a break";
    e.usage = "The company was sued for a breach of contract.";
    m_words.append(e);

    e.word = "Bellow";
    e.definition = "He began to bellow in frustration when he couldn't find the keys.";
    e.translation = "Sigaw\nKasingkahulugan: hiyaw, bulalas, alulong\nKasalungat: bulong, ungol, pabulong\nHalimbawa: Napasigaw siya sa galit nang hindi makita ang kanyang susi.";
    e.synonyms = {"shout", "yell", "roar"};
    e.antonyms = {"whisper", "murmur", "mutter"};
    e.background = "Origin: Old English 'belgan', to swell";
    e.usage = "He began to bellow in frustration when he couldn't find the keys.";
    m_words.append(e);

    e.word = "Betray";
    e.definition = "He felt heartbroken after his best friend betrayed him.";
    e.translation = "Ipagkanulo\nKasingkahulugan: linlangin, traydurin, lokohin\nKasalungat: tulungan, ipagtanggol, maging tapat\nHalimbawa: Labis ang kanyang sama ng loob nang siya'y ipagkanulo ng matalik na kaibigan.";
    e.synonyms = {"deceive", "backstab", "mislead"};
    e.antonyms = {"support", "stand by", "be loyal"};
    e.background = "Origin: Old French 'betrayer', to deliver up";
    e.usage = "He felt heartbroken after his best friend betrayed him.";
    m_words.append(e);

    e.word = "Baggage";
    e.definition = "She packed all her baggage before heading to the airport.";
    e.translation = "Bagahe\nKasingkahulugan: maleta, gamit, dalahin\nKasalungat: wala (depende sa gamit)\nHalimbawa: Ipinakete niya ang lahat ng kanyang bagahe bago pumunta sa paliparan.";
    e.synonyms = {"luggage", "suitcases", "possessions"};
    e.antonyms = {};
    e.background = "Origin: Old French 'bagage', what is carried";
    e.usage = "She packed all her baggage before heading to the airport.";
    m_words.append(e);

    e.word = "Bully";
    e.definition = "He became the target of a bully at school who took his lunch money.";
    e.translation = "Mang-api\nKasingkahulugan: manakot, mang-asar, manggulpi\nKasalungat: ipagtanggol, protektahan, tulungan\nHalimbawa: Naging biktima siya ng mang-aapi na kumukuha ng kanyang baon sa paaralan.";
    e.synonyms = {"intimidate", "harass", "persecute"};
    e.antonyms = {"protect", "defend", "support"};
    e.background = "Origin: Dutch 'boel', lover or brother";
    e.usage = "He became the target of a bully at school who took his lunch money.";
    m_words.append(e);
    
    // Letter C Words
    // -----------------------------------------------------------------
    e.word = "Courageous";
    e.definition = "The courageous soldier saved his comrades under heavy fire.";
    e.translation = "Matapang\nKasingkahulugan: matapang, magiting, walang takot\nKasalungat: duwag, takot, mahiyain\nHalimbawa: Ang matapang na sundalo ay iniligtas ang kanyang mga kasama sa gitna ng putukan.";
    e.synonyms = {"brave", "valiant", "fearless"};
    e.antonyms = {"cowardly", "fearful", "timid"};
    e.background = "Origin: Latin 'coraticus' (via Old French), relating to bravery";
    e.usage = "The courageous soldier saved his comrades under heavy fire.";
    m_words.append(e);

    e.word = "Clever";
    e.definition = "His clever solution to the problem impressed everyone.";
    e.translation = "Matalino\nKasingkahulugan: matalino, madiskarte, marunong\nKasalungat: mangmang, hangal, inosente\nHalimbawa: Ang matalinong solusyon niya sa problema ay ikinamangha ng lahat.";
    e.synonyms = {"smart", "witty", "intelligent"};
    e.antonyms = {"dumb", "foolish", "naive"};
    e.background = "Origin: Old English 'clǣfre', quick to understand";
    e.usage = "His clever solution to the problem impressed everyone.";
    m_words.append(e);

    e.word = "Clumsy";
    e.definition = "She felt clumsy as she tripped over the chair.";
    e.translation = "Tulad ng walang kilos\nKasingkahulugan: awkward, uncoordinated, graceless\nKasalungat: graceful, coordinated, agile\nHalimbawa: Nahulog siya sa upuan dahil sa pagiging clumsy niya.";
    e.synonyms = {"awkward", "uncoordinated", "graceless"};
    e.antonyms = {"graceful", "coordinated", "agile"};
    e.background = "Origin: uncertain, related to lacking coordination";
    e.usage = "She felt clumsy as she tripped over the chair.";
    m_words.append(e);

    e.word = "Cautious";
    e.definition = "The cautious driver slowed down when the weather became foggy.";
    e.translation = "Maingat\nKasingkahulugan: maingat, mapanuri, mapagmatyag\nKasalungat: pabaya, padalos-dalos, walang ingat\nHalimbawa: Ang maingat na drayber ay bumagal nang maging mahamog ang daan.";
    e.synonyms = {"careful", "prudent", "wary"};
    e.antonyms = {"reckless", "careless", "hasty"};
    e.background = "Origin: Latin 'cautus', careful";
    e.usage = "The cautious driver slowed down when the weather became foggy.";
    m_words.append(e);

    e.word = "Charming";
    e.definition = "He was a charming host who made everyone feel welcome.";
    e.translation = "Kaakit-akit\nKasingkahulugan: nakakaaliw, kahali-halina, may karisma\nKasalungat: hindi kaaya-aya, bastos, walang dating\nHalimbawa: Siya ay isang kaakit-akit na host na nagparamdam ng ginhawa sa lahat.";
    e.synonyms = {"delightful", "enchanting", "charismatic"};
    e.antonyms = {"unappealing", "unattractive", "rude"};
    e.background = "Origin: Old French 'charmant', to enchant";
    e.usage = "He was a charming host who made everyone feel welcome.";
    m_words.append(e);

    e.word = "Curious";
    e.definition = "The child was curious about the world around him and asked a lot of questions.";
    e.translation = "Palaisip\nKasingkahulugan: mausisa, interesado, sabik matuto\nKasalungat: walang pakialam, hindi interesado, malamig\nHalimbawa: Ang bata ay palaisip tungkol sa mundo at madalas magtanong.";
    e.synonyms = {"inquisitive", "interested", "eager to learn"};
    e.antonyms = {"indifferent", "uninterested", "apathetic"};
    e.background = "Origin: Latin 'curiosus', inquisitive";
    e.usage = "The child was curious about the world around him and asked a lot of questions.";
    m_words.append(e);

    e.word = "Chilly";
    e.definition = "It was a chilly morning, so I grabbed my jacket before heading out.";
    e.translation = "Malamig\nKasingkahulugan: malamig, presko, maginaw\nKasalungat: mainit, maalinsangan, maalab\nHalimbawa: Maginaw ang umaga kaya nagsuot ako ng jacket bago umalis.";
    e.synonyms = {"cold", "cool", "brisk"};
    e.antonyms = {"warm", "hot", "toasty"};
    e.background = "Origin: Old English 'ciele', chilly";
    e.usage = "It was a chilly morning, so I grabbed my jacket before heading out.";
    m_words.append(e);

    e.word = "Courage";
    e.definition = "It took a lot of courage to speak in front of such a large crowd.";
    e.translation = "Tapang\nKasingkahulugan: bravery, fortitude, valor\nKasalungat: fear, cowardice, timidity\nHalimbawa: Kinailangan ng malaking tapang para magsalita sa harap ng malaking madla.";
    e.synonyms = {"bravery", "fortitude", "valor"};
    e.antonyms = {"fear", "cowardice", "timidity"};
    e.background = "Origin: Latin 'coraticus' via Old French";
    e.usage = "It took a lot of courage to speak in front of such a large crowd.";
    m_words.append(e);

    e.word = "Cynical";
    e.definition = "His cynical attitude made it hard for him to believe in others' goodwill.";
    e.translation = "Mapangduda\nKasingkahulugan: distrustful, skeptical, doubtful\nKasalungat: trusting, hopeful, optimistic\nHalimbawa: Dahil sa kanyang mapangdudang ugali, hirap siyang maniwala sa kabutihan ng iba.";
    e.synonyms = {"distrustful", "skeptical", "doubtful"};
    e.antonyms = {"trusting", "hopeful", "optimistic"};
    e.background = "Origin: Greek 'kynikos', dog-like";
    e.usage = "His cynical attitude made it hard for him to believe in others' goodwill.";
    m_words.append(e);

    e.word = "Compassionate";
    e.definition = "She was a compassionate nurse who always took extra time with her patients.";
    e.translation = "Maawain\nKasingkahulugan: empathetic, kind, caring\nKasalungat: indifferent, apathetic, callous\nHalimbawa: Siya ay isang maawain na nars na laging nagbibigay ng oras sa mga pasyente.";
    e.synonyms = {"empathetic", "kind", "caring"};
    e.antonyms = {"indifferent", "apathetic", "callous"};
    e.background = "Origin: Latin 'compassio', to suffer with";
    e.usage = "She was a compassionate nurse who always took extra time with her patients.";
    m_words.append(e);

    e.word = "Confident";
    e.definition = "She walked into the meeting with a confident attitude.";
    e.translation = "May Kumpiyansa\nKasingkahulugan: self-assured, assertive, positive\nKasalungat: insecure, uncertain, unsure\nHalimbawa: Lumakad siya sa pulong nang may kumpiyansang ugali.";
    e.synonyms = {"self-assured", "assertive", "positive"};
    e.antonyms = {"insecure", "uncertain", "unsure"};
    e.background = "Origin: Latin 'confidere', to trust";
    e.usage = "She walked into the meeting with a confident attitude.";
    m_words.append(e);

    e.word = "Complacent";
    e.definition = "He became complacent after achieving success and stopped working hard.";
    e.translation = "Kuntento sa Sarili\nKasingkahulugan: self-satisfied, content, untroubled\nKasalungat: ambitious, dissatisfied, restless\nHalimbawa: Naging kampante siya matapos magtagumpay at tumigil sa pagsisikap.";
    e.synonyms = {"self-satisfied", "content", "untroubled"};
    e.antonyms = {"ambitious", "dissatisfied", "restless"};
    e.background = "Origin: Latin 'complacere', to please";
    e.usage = "He became complacent after achieving success and stopped working hard.";
    m_words.append(e);

    e.word = "Chaotic";
    e.definition = "The streets were chaotic after the parade, with people everywhere.";
    e.translation = "Magulo\nKasingkahulugan: disorderly, tumultuous, confusing\nKasalungat: orderly, organized, calm\nHalimbawa: Magulo ang mga kalsada matapos ang parada, puno ng tao sa lahat ng dako.";
    e.synonyms = {"disorderly", "tumultuous", "confusing"};
    e.antonyms = {"orderly", "organized", "calm"};
    e.background = "Origin: Greek 'chaos'";
    e.usage = "The streets were chaotic after the parade, with people everywhere.";
    m_words.append(e);

    e.word = "Cumbersome";
    e.definition = "The cumbersome package was hard to carry up the stairs.";
    e.translation = "Mabigat Dalhin\nKasingkahulugan: awkward, unwieldy, bulky\nKasalungat: manageable, easy, simple\nHalimbawa: Ang mabigat na pakete ay mahirap buhatin paakyat ng hagdan.";
    e.synonyms = {"awkward", "unwieldy", "bulky"};
    e.antonyms = {"manageable", "easy", "simple"};
    e.background = "Origin: Old Norse/Old English roots relating to burden";
    e.usage = "The cumbersome package was hard to carry up the stairs.";
    m_words.append(e);

    e.word = "Cautiously";
    e.definition = "He moved cautiously around the broken glass on the floor.";
    e.translation = "Maingat na Paraan\nKasingkahulugan: carefully, warily, prudently\nKasalungat: recklessly, hastily, carelessly\nHalimbawa: Maingat siyang gumalaw sa paligid ng nabasag na baso sa sahig.";
    e.synonyms = {"carefully", "warily", "prudently"};
    e.antonyms = {"recklessly", "hastily", "carelessly"};
    e.background = "Adverbial form of cautious";
    e.usage = "He moved cautiously around the broken glass on the floor.";
    m_words.append(e);

    e.word = "Crucial";
    e.definition = "It is crucial to follow the safety instructions when operating heavy machinery.";
    e.translation = "Mahalaga\nKasingkahulugan: critical, vital, essential\nKasalungat: trivial, insignificant, unimportant\nHalimbawa: Mahalagang sundin ang mga panuntunan sa kaligtasan kapag gumagamit ng mabigat na makina.";
    e.synonyms = {"critical", "vital", "essential"};
    e.antonyms = {"trivial", "insignificant", "unimportant"};
    e.background = "Origin: Greek 'krisis', decisive moment";
    e.usage = "It is crucial to follow the safety instructions when operating heavy machinery.";
    m_words.append(e);

    e.word = "Cleverness";
    e.definition = "Her cleverness in solving the riddle impressed everyone at the party.";
    e.translation = "Katalinuhan\nKasingkahulugan: ingenuity, intelligence, wit\nKasalungat: stupidity, dullness, clumsiness\nHalimbawa: Ang kanyang katalinuhan sa paglutas ng palaisipan ay ikinamangha ng lahat.";
    e.synonyms = {"ingenuity", "intelligence", "wit"};
    e.antonyms = {"stupidity", "dullness", "clumsiness"};
    e.background = "Abstract noun from clever";
    e.usage = "Her cleverness in solving the riddle impressed everyone at the party.";
    m_words.append(e);

    e.word = "Conservative";
    e.definition = "The conservative approach to the project emphasized safety and stability.";
    e.translation = "Konserbatibo\nKasingkahulugan: traditional, cautious, conventional\nKasalungat: liberal, progressive, radical\nHalimbawa: Ang konserbatibong paraan ng proyekto ay nakatuon sa kaligtasan at katatagan.";
    e.synonyms = {"traditional", "cautious", "conventional"};
    e.antonyms = {"liberal", "progressive", "radical"};
    e.background = "Origin: Latin 'conservare', to preserve";
    e.usage = "The conservative approach to the project emphasized safety and stability.";
    m_words.append(e);

    e.word = "Contradictory";
    e.definition = "His contradictory statements left everyone confused about his true intentions.";
    e.translation = "Magkasalungat\nKasingkahulugan: inconsistent, conflicting, opposing\nKasalungat: consistent, harmonious, matching\nHalimbawa: Ang kanyang magkasalungat na pahayag ay nagdulot ng kalituhan.";
    e.synonyms = {"inconsistent", "conflicting", "opposing"};
    e.antonyms = {"consistent", "harmonious", "matching"};
    e.background = "From contra- + dictate, opposing";
    e.usage = "His contradictory statements left everyone confused about his true intentions.";
    m_words.append(e);

    e.word = "Crisis";
    e.definition = "The company faced a crisis after a major financial loss.";
    e.translation = "Krisis\nKasingkahulugan: emergency, disaster, catastrophe\nKasalungat: solution, recovery, resolution\nHalimbawa: Nakaranas ng krisis ang kumpanya matapos ang malaking pagkalugi.";
    e.synonyms = {"emergency", "disaster", "catastrophe"};
    e.antonyms = {"solution", "recovery", "resolution"};
    e.background = "Origin: Greek 'krisis', decision";
    e.usage = "The company faced a crisis after a major financial loss.";
    m_words.append(e);

    e.word = "Competent";
    e.definition = "She is a highly competent manager who always gets the job done.";
    e.translation = "Magaling\nKasingkahulugan: capable, skilled, proficient\nKasalungat: incompetent, unskilled, inept\nHalimbawa: Isa siyang mahusay na tagapamahala na laging natatapos ang gawain.";
    e.synonyms = {"capable", "skilled", "proficient"};
    e.antonyms = {"incompetent", "unskilled", "inept"};
    e.background = "From Latin 'competentia'";
    e.usage = "She is a highly competent manager who always gets the job done.";
    m_words.append(e);

    e.word = "Crude";
    e.definition = "His crude humor made some people uncomfortable at the dinner table.";
    e.translation = "Bastos\nKasingkahulugan: unrefined, rough, vulgar\nKasalungat: refined, sophisticated, polite\nHalimbawa: Ang kanyang bastos na biro ay naka-offend sa ilan sa hapag-kainan.";
    e.synonyms = {"unrefined", "rough", "vulgar"};
    e.antonyms = {"refined", "sophisticated", "polite"};
    e.background = "Origin: Old English 'cruden', raw";
    e.usage = "His crude humor made some people uncomfortable at the dinner table.";
    m_words.append(e);

    e.word = "Calm";
    e.definition = "The calm waters of the lake reflected the evening sky beautifully.";
    e.translation = "Kalma\nKasingkahulugan: peaceful, serene, composed\nKasalungat: agitated, nervous, anxious\nHalimbawa: Ang kalmadong tubig ng lawa ay maganda sa ilalim ng langit sa gabi.";
    e.synonyms = {"peaceful", "serene", "composed"};
    e.antonyms = {"agitated", "nervous", "anxious"};
    e.background = "Origin: Old English 'calm', tranquil";
    e.usage = "The calm waters of the lake reflected the evening sky beautifully.";
    m_words.append(e);

    e.word = "Cleverly";
    e.definition = "He cleverly avoided the question by changing the topic.";
    e.translation = "Matalinong Paraan\nKasingkahulugan: intelligently, wittily, astutely\nKasalungat: foolishly, ineptly, clumsily\nHalimbawa: Matalino niyang naiwasan ang tanong sa pamamagitan ng pagbabago ng paksa.";
    e.synonyms = {"intelligently", "wittily", "astutely"};
    e.antonyms = {"foolishly", "ineptly", "clumsily"};
    e.background = "Adverbial form of clever";
    e.usage = "He cleverly avoided the question by changing the topic.";
    m_words.append(e);

    e.word = "Culminate";
    e.definition = "The event will culminate with a grand fireworks display.";
    e.translation = "Umuabot sa Sukdulan\nKasingkahulugan: conclude, finish, climax\nKasalungat: begin, initiate, start\nHalimbawa: Ang kaganapan ay magtatapos sa isang engrandeng paputok.";
    e.synonyms = {"conclude", "finish", "climax"};
    e.antonyms = {"begin", "initiate", "start"};
    e.background = "From Latin 'culminare', to summit";
    e.usage = "The event will culminate with a grand fireworks display.";
    m_words.append(e);

    e.word = "Challenging";
    e.definition = "The math problem was challenging, but she solved it after a few tries.";
    e.translation = "Mahamon\nKasingkahulugan: difficult, demanding, tough\nKasalungat: easy, simple, effortless\nHalimbawa: Mahirap ang problema sa math, pero nalutas niya pagkatapos ng ilang ulit.";
    e.synonyms = {"difficult", "demanding", "tough"};
    e.antonyms = {"easy", "simple", "effortless"};
    e.background = "Modern English usage";
    e.usage = "The math problem was challenging, but she solved it after a few tries.";
    m_words.append(e);

    e.word = "Corrupt";
    e.definition = "The corrupt officials were arrested after an investigation uncovered their crimes.";
    e.translation = "Tiwali\nKasingkahulugan: dishonest, immoral, depraved\nKasalungat: honest, virtuous, moral\nHalimbawa: Ang mga tiwaling opisyal ay nahuli matapos ang masusing imbestigasyon.";
    e.synonyms = {"dishonest", "immoral", "depraved"};
    e.antonyms = {"honest", "virtuous", "moral"};
    e.background = "From Latin 'corrumpere', to destroy";
    e.usage = "The corrupt officials were arrested after an investigation uncovered their crimes.";
    m_words.append(e);

    e.word = "Conducive";
    e.definition = "The quiet room was conducive to studying and concentration.";
    e.translation = "Nakakatulong\nKasingkahulugan: favorable, helpful, advantageous\nKasalungat: harmful, obstructive, detrimental\nHalimbawa: Ang tahimik na silid ay nakakatulong sa pag-aaral at konsentrasyon.";
    e.synonyms = {"favorable", "helpful", "advantageous"};
    e.antonyms = {"harmful", "obstructive", "detrimental"};
    e.background = "From Latin 'conducere', to lead together";
    e.usage = "The quiet room was conducive to studying and concentration.";
    m_words.append(e);

    e.word = "Contentious";
    e.definition = "The meeting became contentious as both sides refused to compromise.";
    e.translation = "Mapagtalo\nKasingkahulugan: argumentative, controversial, combative\nKasalungat: agreeable, peaceful, harmonious\nHalimbawa: Naging mainit ang pagtatalo dahil parehong ayaw magpatalo ang magkabilang panig.";
    e.synonyms = {"argumentative", "controversial", "combative"};
    e.antonyms = {"agreeable", "peaceful", "harmonious"};
    e.background = "From Latin 'contentio', dispute";
    e.usage = "The meeting became contentious as both sides refused to compromise.";
    m_words.append(e);

    e.word = "Complicated";
    e.definition = "The complicated instructions confused everyone trying to assemble the furniture.";
    e.translation = "Kumplikado\nKasingkahulugan: complex, intricate, convoluted\nKasalungat: simple, straightforward, clear\nHalimbawa: Nalito ang lahat sa komplikadong tagubilin sa pagbuo ng muwebles.";
    e.synonyms = {"complex", "intricate", "convoluted"};
    e.antonyms = {"simple", "straightforward", "clear"};
    e.background = "From Latin 'complicare', to fold together";
    e.usage = "The complicated instructions confused everyone trying to assemble the furniture.";
    m_words.append(e);

    e.word = "Critical";
    e.definition = "Your critical feedback helped improve the quality of the final report.";
    e.translation = "Kritikal\nKasingkahulugan: essential, crucial, urgent\nKasalungat: insignificant, trivial, unimportant\nHalimbawa: Ang iyong kritikal na puna ay nakatulong sa pagpapabuti ng ulat.";
    e.synonyms = {"essential", "crucial", "urgent"};
    e.antonyms = {"insignificant", "trivial", "unimportant"};
    e.background = "From Greek 'kritikos', able to judge";
    e.usage = "Your critical feedback helped improve the quality of the final report.";
    m_words.append(e);

    e.word = "Credible";
    e.definition = "The journalist gave a credible account of the events that took place.";
    e.translation = "Mapagkakatiwalaan\nKasingkahulugan: believable, reliable, trustworthy\nKasalungat: unbelievable, unreliable, dubious\nHalimbawa: Nagbigay ang mamamahayag ng kapanipaniwalang salaysay ng mga pangyayari.";
    e.synonyms = {"believable", "reliable", "trustworthy"};
    e.antonyms = {"unbelievable", "unreliable", "dubious"};
    e.background = "From Latin 'credibilis', believable";
    e.usage = "The journalist gave a credible account of the events that took place.";
    m_words.append(e);

    e.word = "Clarity";
    e.definition = "The clarity of her explanation made the complex concept easy to understand.";
    e.translation = "Kalinawan\nKasingkahulugan: clearness, precision, transparency\nKasalungat: confusion, ambiguity, vagueness\nHalimbawa: Dahil sa linaw ng kanyang paliwanag, madali nilang naintindihan ang konsepto.";
    e.synonyms = {"clearness", "precision", "transparency"};
    e.antonyms = {"confusion", "ambiguity", "vagueness"};
    e.background = "From Latin 'claritas', brightness";
    e.usage = "The clarity of her explanation made the complex concept easy to understand.";
    m_words.append(e);

    e.word = "Crowded";
    e.definition = "The subway was crowded during rush hour, making it difficult to move.";
    e.translation = "Matao\nKasingkahulugan: packed, congested, jammed\nKasalungat: empty, spacious, vacant\nHalimbawa: Matao sa tren tuwing rush hour kaya hirap gumalaw.";
    e.synonyms = {"packed", "congested", "jammed"};
    e.antonyms = {"empty", "spacious", "vacant"};
    e.background = "Common modern English";
    e.usage = "The subway was crowded during rush hour, making it difficult to move.";
    m_words.append(e);

    e.word = "Circular";
    e.definition = "The park had a circular walking path that looped around the lake.";
    e.translation = "Bilog\nKasingkahulugan: round, ring-shaped, annular\nKasalungat: square, rectangular, angular\nHalimbawa: May paikot na daanan sa parke na pumapalibot sa lawa.";
    e.synonyms = {"round", "ring-shaped", "annular"};
    e.antonyms = {"square", "rectangular", "angular"};
    e.background = "From Latin 'circulus', small ring";
    e.usage = "The park had a circular walking path that looped around the lake.";
    m_words.append(e);

    e.word = "Cuddly";
    e.definition = "The cuddly kitten purred as it curled up in my lap.";
    e.translation = "Malambing\nKasingkahulugan: soft, huggable, snuggly\nKasalungat: rough, stiff, uninviting\nHalimbawa: Ang malambing na kuting ay umidlip sa aking kandungan.";
    e.synonyms = {"soft", "huggable", "snuggly"};
    e.antonyms = {"rough", "stiff", "uninviting"};
    e.background = "Colloquial usage";
    e.usage = "The cuddly kitten purred as it curled up in my lap.";
    m_words.append(e);

    e.word = "Clamorous";
    e.definition = "The clamorous crowd cheered as the team scored the winning goal.";
    e.translation = "Maingay\nKasingkahulugan: noisy, loud, boisterous\nKasalungat: quiet, peaceful, subdued\nHalimbawa: Sigawan ang narinig mula sa maingay na karamihan matapos manalo ang koponan.";
    e.synonyms = {"noisy", "loud", "boisterous"};
    e.antonyms = {"quiet", "peaceful", "subdued"};
    e.background = "From Latin 'clamor', a shout";
    e.usage = "The clamorous crowd cheered as the team scored the winning goal.";
    m_words.append(e);

    e.word = "Cold";
    e.definition = "The cold wind made it feel like winter even though it was still autumn.";
    e.translation = "Malamig\nKasingkahulugan: chilly, frigid, icy\nKasalungat: warm, hot, toasty\nHalimbawa: Ang malamig na hangin ay nagparamdam ng taglamig kahit taglagas pa.";
    e.synonyms = {"chilly", "frigid", "icy"};
    e.antonyms = {"warm", "hot", "toasty"};
    e.background = "Old English 'cald'";
    e.usage = "The cold wind made it feel like winter even though it was still autumn.";
    m_words.append(e);

    e.word = "Capable";
    e.definition = "She is capable of handling complex tasks under pressure.";
    e.translation = "May Kakayahan\nKasingkahulugan: competent, skilled, able\nKasalungat: incompetent, incapable, unfit\nHalimbawa: Marunong siyang humawak ng mahihirap na gawain kahit may pressure.";
    e.synonyms = {"competent", "skilled", "able"};
    e.antonyms = {"incompetent", "incapable", "unfit"};
    e.background = "From Latin 'capax', able to contain";
    e.usage = "She is capable of handling complex tasks under pressure.";
    m_words.append(e);

    e.word = "Captive";
    e.definition = "The animals in the zoo were captive, unable to roam free in the wild.";
    e.translation = "Bilanggo\nKasingkahulugan: imprisoned, confined, enslaved\nKasalungat: free, liberated, independent\nHalimbawa: Ang mga hayop sa zoo ay bilanggo at hindi makagala sa kagubatan.";
    e.synonyms = {"imprisoned", "confined", "enslaved"};
    e.antonyms = {"free", "liberated", "independent"};
    e.background = "From Latin 'captivus', taken";
    e.usage = "The animals in the zoo were captive, unable to roam free in the wild.";
    m_words.append(e);

    e.word = "Clear";
    e.definition = "The instructions were clear, and everyone understood what to do.";
    e.translation = "Malinaw\nKasingkahulugan: obvious, evident, transparent\nKasalungat: unclear, ambiguous, opaque\nHalimbawa: Malinaw ang mga tagubilin kaya lahat ay nakasunod.";
    e.synonyms = {"obvious", "evident", "transparent"};
    e.antonyms = {"unclear", "ambiguous", "opaque"};
    e.background = "From Old English 'cleare'";
    e.usage = "The instructions were clear, and everyone understood what to do.";
    m_words.append(e);

    e.word = "Charitable";
    e.definition = "The charitable organization helps provide food and shelter for the homeless.";
    e.translation = "Mapagbigay\nKasingkahulugan: generous, kind, benevolent\nKasalungat: selfish, greedy, stingy\nHalimbawa: Ang mapagbigay na organisasyon ay tumutulong sa mga walang tirahan.";
    e.synonyms = {"generous", "kind", "benevolent"};
    e.antonyms = {"selfish", "greedy", "stingy"};
    e.background = "From Latin 'caritas', charity";
    e.usage = "The charitable organization helps provide food and shelter for the homeless.";
    m_words.append(e);

    e.word = "Content";
    e.definition = "After a long day of work, he felt content sitting on the couch.";
    e.translation = "Kuntento\nKasingkahulugan: satisfied, pleased, happy\nKasalungat: dissatisfied, unhappy, discontent\nHalimbawa: Pagkatapos ng buong araw ng trabaho, kuntento siyang naupo sa sofa.";
    e.synonyms = {"satisfied", "pleased", "happy"};
    e.antonyms = {"dissatisfied", "unhappy", "discontent"};
    e.background = "From Latin 'contentus', satisfied";
    e.usage = "After a long day of work, he felt content sitting on the couch.";
    m_words.append(e);

    e.word = "Conserve";
    e.definition = "We need to conserve water during the drought to avoid shortages.";
    e.translation = "Magtipid\nKasingkahulugan: preserve, protect, save\nKasalungat: waste, squander, deplete\nHalimbawa: Kailangang magtipid ng tubig sa panahon ng tagtuyot.";
    e.synonyms = {"preserve", "protect", "save"};
    e.antonyms = {"waste", "squander", "deplete"};
    e.background = "From Latin 'conservare'";
    e.usage = "We need to conserve water during the drought to avoid shortages.";
    m_words.append(e);

    e.word = "Commendable";
    e.definition = "Her commendable efforts to reduce waste in the office were recognized by management.";
    e.translation = "Kapuri-puri\nKasingkahulugan: praiseworthy, admirable, laudable\nKasalungat: disreputable, dishonorable, blameworthy\nHalimbawa: Ang kanyang kapuri-puring pagsisikap na bawasan ang basura ay napansin ng pamunuan.";
    e.synonyms = {"praiseworthy", "admirable", "laudable"};
    e.antonyms = {"disreputable", "dishonorable", "blameworthy"};
    e.background = "From Latin 'commendare', to entrust";
    e.usage = "Her commendable efforts to reduce waste in the office were recognized by management.";
    m_words.append(e);

    e.word = "Composed";
    e.definition = "Despite the chaos around her, she remained composed and kept working.";
    e.translation = "Kalma\nKasingkahulugan: calm, collected, serene\nKasalungat: agitated, nervous, stressed\nHalimbawa: Sa kabila ng kaguluhan, nanatili siyang kalmado at nagpatuloy sa trabaho.";
    e.synonyms = {"calm", "collected", "serene"};
    e.antonyms = {"agitated", "nervous", "stressed"};
    e.background = "From Latin 'componere', to put together";
    e.usage = "Despite the chaos around her, she remained composed and kept working.";
    m_words.append(e);

    // Post-process initial words to ensure each entry has a distinct definition and usage.
    // This prevents the UI from showing identical definition and usage.
    for (WordEntry &we : m_words) {
        // Ensure definition exists
        if (we.definition.trimmed().isEmpty()) {
            we.definition = QString("Definition for %1 is not available.").arg(we.word);
        }

        // If usage is empty or identical to definition, generate a short example
        if (we.usage.trimmed().isEmpty() || we.usage.trimmed() == we.definition.trimmed()) {
            const QString def = we.definition.trimmed();
            if (!def.isEmpty()) {
                // Create a different-looking example sentence while reusing the meaning
                we.usage = QString("In context, '%1' can be used like this: %2").arg(we.word, def);
            } else {
                we.usage = QString("Example: '%1' used in a sentence.").arg(we.word);
            }
        }

        // If background is missing this function provides a small placeholder that still looks informative
        if (we.background.trimmed().isEmpty()) {
            we.background = QString("Background: Etymology or usage information for '%1' is not provided.").arg(we.word);
        }
    }

}

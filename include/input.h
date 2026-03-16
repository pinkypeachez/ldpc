#pragma once

class MessageDispatcher {

    private:
        std::string userInput; 
        std::vector<std::array<uint64_t, 4>> chunks_; // 4 * 64bit = 256bit pro Chunk
        //size_t current_chunk_ = 0; // nur diese Klasse darf den index erhöhen wobei eig nicht nötig
        size_t n_batch_ = 0;


        void FillChunksWithInput() {

            //Wie viele "message" Blöcke sollen dem LDPC gegeben werden?
            n_batch_ = std::ceil(this->userInput.size()/(32.0f)); //32 weil 256 message bit = 32 Byte sind
//ab hier weiter machen
        }

    public:

    void dispatch(const std::string& userInput) {
        this->userInput = userInput; // speichern für die func um beim decodieren wieder als ascii formatiert auszugeben
        FillChunksWithInput(); 
        std::cout << "Die Nachricht wurde für Encoder vorbereitet. Es wird  " << n_batch_ << " Durchläufe geben" << std::endl;
    }


}
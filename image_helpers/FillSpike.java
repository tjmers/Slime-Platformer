package image_helpers;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class FillSpike {
    public static void main(String[] args) {
        fillSpike();
    }

    static void fillSpike() {
        BufferedImage image = null;
        try {
            File input = new File(".\\images\\spikes.png");
            image = ImageIO.read(input);
        } catch (IOException e) {
            e.printStackTrace();
        }
        int onePart = 255 / 64;
        for (int y = 0; y < 32; ++y) {
            boolean black = false;
            for (int x = 0; x < 32; ++x) {
                if (image.getRGB(x, y) == Color.BLACK.getRGB()) {
                    black = !black;
                    while (image.getRGB(x, y) == Color.BLACK.getRGB()) {
                        ++x;
                    }
                    --x;
                    continue;
                }
                if (black) {
                    image.setRGB(x, y, (onePart * (63 - y)) << 24);
                }
            }
        }


        try {
            File output = new File(".\\images\\new_spikes.png");
            ImageIO.write(image, "png", output);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}

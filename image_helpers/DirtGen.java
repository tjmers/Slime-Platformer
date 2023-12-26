package image_helpers;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

// this is mostly just a copy paste from StoneGen.java

import javax.imageio.ImageIO;

public class DirtGen {
    public static void main(String[] args) {
        generate();
    }

    static void generate() {
        BufferedImage image = new BufferedImage(32, 32, BufferedImage.TYPE_INT_RGB);
        
        Color[] colors = {new Color(60, 39, 18), new Color(48, 32, 14), new Color(43, 27, 12), new Color(41, 26, 10)};
        
        // generate the image
        for (int i = 0; i < image.getWidth(); ++i) {
            for (int j = 0; j < image.getHeight(); ++j) {
                image.setRGB(i, j, colors[(int)(Math.random() * colors.length)].getRGB());
            }
        }

        save(image);
        return;
        
    }
    
    static void save(BufferedImage image) {
        try {
            File outputFile = new File("dirt.png");
            ImageIO.write(image, "png", outputFile);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

}
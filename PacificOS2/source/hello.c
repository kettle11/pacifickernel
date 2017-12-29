#include <efi.h>
#include <efilib.h>
#include "graphics.h"

#define DESIRED_HREZ            1024
#define DESIRED_VREZ             768
#define DESIRED_PIXEL_FORMAT    PixelBlueGreenRedReserved8BitPerColor

void PreBootHalt( EFI_SIMPLE_TEXT_OUT_PROTOCOL* conerr, UINT16* msg );
CHAR16 * itoa( UINT32 value, CHAR16 * str, UINT32 base );
//void drawTriangle( EFI_PHYSICAL_ADDRESS lfb_base_addr, UINTN center_x, UINTN center_y, UINTN width, UINT32 color );

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    //EFI_SYSTEM_TABLE   *gST;
    EFI_BOOT_SERVICES  *gBS;

    EFI_HANDLE* handle_buffer;
    UINTN handle_count = 0;

    UINTN mode_num;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gop_mode_info;

    UINTN size_of_info;

    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
  
    /* Store the system table for future use in other functions */
    ST = SystemTable;
    
     if (!(gBS = SystemTable->BootServices))
        return EFI_LOAD_ERROR;

    /* Say hi */
    Status = ST->ConOut->OutputString(ST->ConOut, L"Hello Pacific\n\r");
    if (EFI_ERROR(Status))
        return Status;

    Status = gBS->LocateHandleBuffer( ByProtocol,
                                      &gEfiGraphicsOutputProtocolGuid,
                                      NULL,
                                      &handle_count,
                                      &handle_buffer );

    if (Status != EFI_SUCCESS)
        PreBootHalt( ST->ConOut, L"LocateHandleBuffer() failed" );

    ST->ConOut->OutputString(ST->ConOut, L"TEST");


    // Graphics stuff
    Status = gBS->HandleProtocol( handle_buffer[0],
                                  &gEfiGraphicsOutputProtocolGuid,
                                  (VOID **)&gop );

    if (Status != EFI_SUCCESS)
    {
        ST->ConOut->OutputString(ST->ConOut, L"Handle Protocol failed!");
        PreBootHalt( ST->ConOut, L"HandleProtocol() failed" );
    }

    int imax = gop->Mode->MaxMode;
 
    Print(L"GOP reports MaxMode %d\n", imax);

    for (mode_num = 0;
         (Status = gop->QueryMode( gop, mode_num, &size_of_info, &gop_mode_info )) == EFI_SUCCESS;
         mode_num++) {

        CHAR16* intToString;
        itoa(gop_mode_info->HorizontalResolution, intToString, 10);

        ST->ConOut->OutputString(ST->ConOut, L"\n\r");
        ST->ConOut->OutputString(ST->ConOut, intToString);
        ST->ConOut->OutputString(ST->ConOut, L"  ");
        ST->ConOut->OutputString(ST->ConOut, itoa(gop_mode_info->VerticalResolution, intToString, 10));

        ST->ConOut->OutputString(ST->ConOut, L"  ");
        ST->ConOut->OutputString(ST->ConOut, itoa(gop_mode_info->PixelFormat, intToString, 10));

        ST->ConOut->OutputString(ST->ConOut, L"\n\r");

        if (gop_mode_info->HorizontalResolution == DESIRED_HREZ &&
              gop_mode_info->VerticalResolution == DESIRED_VREZ &&
              gop_mode_info->PixelFormat        == DESIRED_PIXEL_FORMAT)
            break;
    }

    
    if (Status != EFI_SUCCESS)
        PreBootHalt( ST->ConOut, L"Failed to find desired mode" );

    if (gop->SetMode( gop, mode_num) != EFI_SUCCESS)
        PreBootHalt( ST->ConOut, L"SetMode() failed" );

    ST->ConOut->ClearScreen(ST->ConOut);

    setResolution(DESIRED_HREZ, DESIRED_VREZ);
    setFrameBuffer(gop->Mode->FrameBufferBase);
    drawTriangle( DESIRED_HREZ / 2, DESIRED_VREZ / 2 - 25, 100, 0x00ff99ff );
    drawRectangle(100, 100, 300, 300, 0x004E7BFE);
    drawRectangle(500, 100, 100, 700, 0x004E7BFE);

    drawString(130, 500, "The quick brown fox jumps over the lazy dog!");

    /* Now wait for a keystroke before continuing, otherwise your
       message will flash off the screen before you see it.
 
       First, we need to empty the console input buffer to flush
       out any keystrokes entered before this point */
    Status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(Status))
        return Status;
 
    /* Now wait until a key becomes available.  This is a simple
       polling implementation.  You could try and use the WaitForKey
       event instead if you like */
    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ;

    return Status;
}


void PreBootHalt( EFI_SIMPLE_TEXT_OUT_PROTOCOL* conerr, UINT16* msg ) {
    conerr->OutputString( conerr, msg );
    for (;;) ;
}

CHAR16 * itoa( UINT32 value, CHAR16 * str, UINT32 base )
{
    CHAR16 * rc;
    CHAR16 * ptr;
    CHAR16 * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        CHAR16 tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

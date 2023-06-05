import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { AlertController, IonicModule } from '@ionic/angular';
import { CodeService } from '../code.service';

@Component({
  selector: 'app-new-pin',
  templateUrl: './new-pin.page.html',
  styleUrls: ['./new-pin.page.scss'],
  standalone: true,
  imports: [IonicModule, CommonModule, FormsModule]
})
export class NewPinPage implements OnInit {

  newPin = "";
  oldPin = '';
  constructor(private codeService: CodeService,
              private alertController: AlertController) { }

  ngOnInit() {
    this.oldPin = this.codeService.expectedCode
  }

  onInput(event: Event) {
    //@ts-ignore
    this.newPin = event.detail.value
  }

  setPin() {
    if (this.newPin != "") {
      this.codeService.expectedCode = this.newPin
      console.log(this.codeService.expectedCode)
      this.showOk()
    } else {
      this.showAlert()
    }
  }

  showOk() {

    this.alertController.create({
      header: 'Great :)',
      subHeader: 'You have a new pin.',
      message: `Succesfully set ${this.codeService.expectedCode} as your new pin.`,
      buttons: ['OK']
    }).then(res => {
      res.present();
    });
  }

  showAlert() {
    this.alertController.create({
      header: 'Error :(',
      subHeader: 'Wrong pin!',
      message: 'Please type a new value.',
      buttons: ['OK']
    }).then(res => {
      res.present();
    });
  }
}
